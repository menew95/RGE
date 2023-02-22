#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"
#include "Header/H_Light.hlsli"
#include "Header/H_Shadow.hlsli"

Texture2D gAlbedo	: register(t0);
Texture2D gNormal	: register(t1);
Texture2D gDepth	: register(t2);
Texture2D gWorldPos	: register(t3);
Texture2D gEmissive	: register(t4);

TextureCube gPreFilteredMap : register(t5);
TextureCube gIrradianceMap : register(t6);
Texture2D gIntegrateBRDFMap : register(t7);

Texture2DArray gCascadedShadowMap : register(t8);

Texture2DArray gSpotShadowMap : register(t9);

TextureCubeArray gPointShadowMap : register(t10);

Texture2D gReflect : register(t11);

SamplerState samWrapLinear	: register(s0);

SamplerComparisonState samShadowSampler : register(s1);

SamplerState samPointLight : register(s2);

SamplerState samPoint : register(s3);

float3 CalcIBL(float3 V, float3 N, float3 albedo, float3 F0, float roughness, float metallic, float ao)
{
	float3 R = reflect(-V, N);

	const float MAX_REFLECTION_LOD = 5.0;
	float3 prefilteredColor = gPreFilteredMap.SampleLevel(samWrapLinear, R, roughness * 5).rgb;

	float3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	float3 kS = F;
	float3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;

	float3 irradiance = gIrradianceMap.Sample(samWrapLinear, N).rgb;
	float3 diffuse = irradiance * albedo;

	float2 envBRDF = gIntegrateBRDFMap.Sample(samWrapLinear, float2(max(dot(N, V), 0.0), roughness)).rg;
	float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	float3 ambient = (kD * diffuse + specular) * ao;

	return ambient;
}

float4 main(VSOutput input) : SV_TARGET
{
	float3 _finColor = { 0.0f, 0.0f, 0.0f };

	float4 depth = gDepth.Sample(samWrapLinear, input.uv);

	clip(depth.x == 1.0f ? -1 : 1);

	float4 _albedo = gAlbedo.Sample(samWrapLinear, input.uv);
	float4 _normal = gNormal.Sample(samWrapLinear, input.uv);
	float4 _worldPos = gWorldPos.Sample(samWrapLinear, input.uv);
	float4 _emissive = gEmissive.Sample(samWrapLinear, input.uv);

	float _roughness = lerp(0.04f, 1.0f, _normal.w);
	float _metallic = lerp(0.04f, 1.0f, _albedo.w);
	float _ao = _worldPos.a;

	_worldPos.a = 1.0f;

	_albedo.w = 1.0f;
	_albedo = pow(_albedo, 2.2);

	/*_normal.w = 1.0f;
	_normal = (_normal - 0.5f) * 2.0f;*/
	float3 N = ((_normal - 0.5f) * 2.0f).xyz;
	float3 V = normalize(camera._camWorld.xyz - _worldPos.xyz);

	// Specular coefficiant - fixed reflectance value for non-metals
	static const float kSpecularCoefficient = 0.04;

	float3 _specularColor = lerp(kSpecularCoefficient, _albedo.xyz, _metallic);
	float3 _diffuseColor = lerp(_albedo.xyz, float3(0, 0, 0), _metallic);

	float3 _lightColor = float3(0.0f, 0.0f, 0.0f);


	for (uint _dirIdx = 0; _dirIdx < _lightCount.x; _dirIdx++)
	{
		float _shadowFactor = 1.0f;
		float4 cascadeLightPos[4];
		float _clipZ = _emissive.a;

		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			cascadeLightPos[i] = mul(_worldPos, _cascadedLight._lightTransform[i]);
		}

		[unroll]
		for (int j = 0; j < 4; ++j)
		{
			if (_clipZ <= _cascadedLight._cascadeEndClipSpace[j])
			{
				_shadowFactor = CalcCascadeShadowFactor(j, cascadeLightPos[j], gCascadedShadowMap, samShadowSampler);
				break;
			}
		}

		_lightColor += CalcDirectionalLight(_directionLight[_dirIdx], _roughness, _metallic, _specularColor, _diffuseColor, N, V) * _shadowFactor;
	}

	for (uint _pointIdx = 0; _pointIdx < _lightCount.y; _pointIdx++)
	{
		float _shadowFactor = 1.0f;

		uint _view = SelectView(_worldPos.xyz, _pointLight[_pointIdx]._position);

		float4 _lightSpacePos = mul(_worldPos, _pointLight[_pointIdx]._lightTransform[_view]);

		float3 _posToLight = _worldPos.xyz - _pointLight[_pointIdx]._position;
		
		_shadowFactor = CalcPointShadowFactor(_pointIdx, _lightSpacePos, _posToLight, _pointLight[_pointIdx]._range, gPointShadowMap, samShadowSampler);

		_lightColor += CalcPointLight(_pointLight[_pointIdx], _roughness, _metallic, _specularColor, _diffuseColor, N, V, _worldPos.xyz) * _shadowFactor;
	}
	
	for (uint _spotIdx = 0; _spotIdx < _lightCount.z; _spotIdx++)
	{
		float _shadowFactor = 1.0f;

		float4 _lightSpacePos = mul(_worldPos, _spotLight[_spotIdx]._lightTransform);

		_shadowFactor = CalcSpotShadowFactor(_spotIdx, _lightSpacePos, gSpotShadowMap, samShadowSampler);

		_lightColor += CalcSpotLight(_spotLight[_spotIdx], _roughness, _metallic, _specularColor, _diffuseColor, N, V, _worldPos.xyz) * _shadowFactor;
	}
	
	//float3 _ambient = CalcIBL(V, N, _albedo.rgb, _specularColor, _roughness, _metallic, _ao);
	float4 _reflect = gReflect.Sample(samPoint, input.uv);
	int a = 0;

	float3 _ambient = gAlbedo.Sample(samPoint, _reflect.xy);

	_finColor = _ambient * _reflect.w;// +_lightColor;

	_finColor = pow(_finColor, 1 / 2.2);

	return float4(_finColor, 1.0f);
}