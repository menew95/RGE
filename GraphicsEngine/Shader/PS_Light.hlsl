#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"
#include "Header/H_Light.hlsli"

Texture2D gAlbedo	: register(t0);
Texture2D gNormal	: register(t1);
Texture2D gDepth	: register(t2);
Texture2D gWorldPos	: register(t3);
Texture2D gEmissive	: register(t4);

SamplerState samWrapLinear	: register(s0);

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

	_albedo.w = 1.0f;
	_albedo = pow(_albedo, 2.2);

	_normal.w = 1.0f;
	_normal = (_normal - 0.5f) * 2.0f;

	float3 _toEye = normalize(camera._camWorld.xyz - _worldPos.xyz);

	float _roughness2 = max(0.001, _roughness * _roughness);


	// Specular coefficiant - fixed reflectance value for non-metals
	static const float kSpecularCoefficient = 0.04;

	float3 _specularColor = lerp(kSpecularCoefficient, _albedo.xyz, _metallic);
	float3 _diffuseColor = lerp(_albedo.xyz, float3(0, 0, 0), _metallic);

	//[unroll]
	for (uint _lightIdx = 0; _lightIdx < LightCount; _lightIdx++)
	{
		_finColor += CalLight(LightInfo[_lightIdx], _specularColor, _diffuseColor, _worldPos.xyz, _normal.xyz, _toEye, _roughness2, _metallic);
	}

	return float4(_finColor, 1.0f);
}