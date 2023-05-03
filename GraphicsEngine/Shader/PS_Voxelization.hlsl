#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Input.hlsli"
#include "Header\H_Voxel.hlsli"
#include "Header\H_Math.hlsli"

#include "Header\H_Light.hlsli"
#include "Header\H_Shadow.hlsli"

#if defined(_ALBEDO_MAP)
Texture2D gAlbedoMap	: register(t0);
#endif

#if defined(_NORMAL_MAP)
Texture2D gBumpMap		: register(t1);
#endif

#if defined(_MRA_MAP)
Texture2D gMRAMap		: register(t2);
#endif

TextureCube gPreFilteredMap : register(t5);
TextureCube gIrradianceMap : register(t6);
Texture2D gIntegrateBRDFMap : register(t7);

Texture2DArray gCascadedShadowMap : register(t8);

Texture2DArray gSpotShadowMap : register(t9);

TextureCubeArray gPointShadowMap : register(t10);

SamplerState samWrapLinear	: register(s0);

SamplerComparisonState samShadowSampler : register(s1);

SamplerState samPointLight : register(s2);

SamplerState samPoint : register(s3);

struct PSInput
{
    float4 posH     : SV_POSITION;
    float4 posW     : POSITION1;
    float4 color	: COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;

#if defined(_NORMAL_MAP)
    float3 tangent	: TANGENT;
#endif
};

RWStructuredBuffer<VoxelType> VoxelGrid     : register(u0);

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

void main(PSInput input)
{
    float3 _diff = (input.posW.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataResRCP * voxel_radiance._dataSizeRCP;
    float3 _voxelUVW = _diff * float3(0.5f, -0.5f, 0.5f) + 0.5f; // 복셀 텍스처 좌표계로 변환

    [branch]
    if (any(0 > _voxelUVW || 1 <= _voxelUVW))
    {
        return;
    }

#if !defined(_NORMAL_MAP)
    float3 _normal = input.normal;
#else
    float3 normalMapSample = gBumpMap.Sample(samWrapLinear, input.uv).rgb;

    float3 bumpedNormalW = { 0.0f, 0.0f, 0.0f };

    NormalSampleToWorldSpace(bumpedNormalW, normalMapSample, input.normal, input.tangent);

    float3 _normal = bumpedNormalW;
#endif

#if !defined(_MRA_MAP)
    float _metallic = lerp(0.04f, 1.0f, material._metallic);

    float _roughness = material._roughness;

    float _ao = 1.0f;
#else
    float3 _MRA = gMRAMap.Sample(samWrapLinear, input.uv).rgb;

    // texture의 color공간이 어디서 만들어지냐에 따라 필요 할 수도 있고 안해도 될 수도 있다. 
    //_MRA = pow(_MRA, 2.2);

    float _metallic = lerp(0.04f, 1.0f, _MRA.r);

    float _roughness = _MRA.g;

    float _ao = _MRA.b;
#endif

#if !defined(_ALBEDO_MAP)
    float4 _albedo = material._albedo;
#else
    float4 _albedo = gAlbedoMap.Sample(samWrapLinear, input.uv);
#endif

    float4 _emissive = material._emissive;


    // 최종 색상값 Forward 연산

    float3 _finColor = { 0.0f, 0.0f, 0.0f };
    float4 _worldPos = input.posW;

    // Gamma
    _albedo = pow(_albedo, 2.2);

    float3 N = _normal;
    float3 V = normalize(camera._camWorld.xyz - input.posW.xyz);

    static const float kSpecularCoefficient = 0.04;

    float3 _specularColor = lerp(kSpecularCoefficient, _albedo.xyz, _metallic);
    float3 _diffuseColor = lerp(_albedo.xyz, float3(0, 0, 0), _metallic);

    float3 _lightColor = float3(0.0f, 0.0f, 0.0f);

    // calculate directional light
    [unroll]
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

    // calculate spot light
    [unroll]
    for (uint _pointIdx = 0; _pointIdx < _lightCount.y; _pointIdx++)
    {
        float _shadowFactor = 1.0f;

        uint _view = SelectView(_worldPos.xyz, _pointLight[_pointIdx]._position);

        float4 _lightSpacePos = mul(_worldPos, _pointLight[_pointIdx]._lightTransform[_view]);

        float3 _posToLight = _worldPos.xyz - _pointLight[_pointIdx]._position;

        _shadowFactor = CalcPointShadowFactor(_pointIdx, _lightSpacePos, _posToLight, _pointLight[_pointIdx]._range, gPointShadowMap, samShadowSampler);

        _lightColor += CalcPointLight(_pointLight[_pointIdx], _roughness, _metallic, _specularColor, _diffuseColor, N, V, _worldPos.xyz) * _shadowFactor;
    }

    // calculate spot light
    [unroll]
    for (uint _spotIdx = 0; _spotIdx < _lightCount.z; _spotIdx++)
    {
        float _shadowFactor = 1.0f;

        float4 _lightSpacePos = mul(_worldPos, _spotLight[_spotIdx]._lightTransform);

        _shadowFactor = CalcSpotShadowFactor(_spotIdx, _lightSpacePos, gSpotShadowMap, samShadowSampler);

        _lightColor += CalcSpotLight(_spotLight[_spotIdx], _roughness, _metallic, _specularColor, _diffuseColor, N, V, _worldPos.xyz) * _shadowFactor;
    }

    float3 _ambient = CalcIBL(V, N, _albedo.rgb, _specularColor, _roughness, _metallic, _ao);

    _finColor = _ambient + _lightColor;

    // Gamma
    _finColor = pow(_finColor, 1 / 2.2);


    // Voxel Encode
    float4 _color = float4(_finColor, 1.0f);

    uint _colorEncoded = EncodeColor(_color);
    uint _normalEncoded = EncodeNormal(_normal);

    uint3 _writecoord = floor(_voxelUVW * voxel_radiance._dataRes);
    uint _id = Flatten3D(_writecoord, voxel_radiance._dataRes);

    InterlockedMax(VoxelGrid[_id]._colorMask, _colorEncoded);
    InterlockedMax(VoxelGrid[_id]._normalMask, _normalEncoded);
}