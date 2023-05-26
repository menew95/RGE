#include "Header/H_Voxel.hlsli"
#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"
#include "Header/H_Light.hlsli"
#include "Header/H_Shadow.hlsli"
#include "Header/H_Math.hlsli"

static const float3 diffuseConeDirections[] =
{
    float3(0.0f , 1.0f , 0.0f) ,
    float3(0.0f , 0.5f , 0.866025f) ,
    float3(0.823639f , 0.5f , 0.267617f) ,
    float3(0.509037f , 0.5f , -0.7006629f) ,
    float3(-0.50937f , 0.5f , -0.7006629f) ,
    float3(-0.823639f , 0.5f , 0.267617f)
};

static const float diffuseConeWeights[] =
{
    PI / 4.0f ,
    3.0f * PI / 20.0f ,
    3.0f * PI / 20.0f ,
    3.0f * PI / 20.0f ,
    3.0f * PI / 20.0f ,
    3.0f * PI / 20.0f ,
};

static const float sqrt2 = 1.414213562;
Texture2D gAlbedo	: register(t0);
Texture2D gNormal   : register(t1);
Texture2D<float> gDepth    : register(t2);
Texture2D gWorldPos	: register(t3);
Texture3D<float4> voxelTexture : register(t4);
Texture3D<float4> voxelTexMipmap[6] : register(t5);
Texture2D<float4> gDirectionLight : register(t11);
Texture2D<float4> gIntegrateBRDFMap : register(t12);

SamplerState samWrapLinear	: register(s0);
SamplerState samClampLinear	: register(s1);

float4 AnistropicSample(in float3 coord, in float3 weight, in float3 dir, in float lod)
{
    // �̹漺 ���� LOD
    float anisoLevel = max(lod - 1.0f, 0.0f);

    // ���⼺ ���ø�
    float4 anisoSample = float4(0.f, 0.f, 0.f, 0.f);

    if (dir.x < 0.0)
    {
        anisoSample += weight.x * voxelTexMipmap[0].SampleLevel(samClampLinear, coord, anisoLevel);
    }
    else
    {
        anisoSample += weight.x * voxelTexMipmap[1].SampleLevel(samClampLinear, coord, anisoLevel);
    }

    [branch]
    if (dir.y < 0.0)
    {
        anisoSample += weight.y * voxelTexMipmap[2].SampleLevel(samClampLinear, coord, anisoLevel);
    }
    else
    {
        anisoSample += weight.y * voxelTexMipmap[3].SampleLevel(samClampLinear, coord, anisoLevel);
    }

    if (dir.z < 0.0)
    {
        anisoSample += weight.z * voxelTexMipmap[4].SampleLevel(samClampLinear, coord, anisoLevel);
    }
    else
    {
        anisoSample += weight.z * voxelTexMipmap[5].SampleLevel(samClampLinear, coord, anisoLevel);
    }

    // ���� 0�� ��� �������� ó��
    if (lod < 1.0f)
    {
        float4 baseColor = voxelTexture.SampleLevel(samClampLinear, coord, 0);
        anisoSample = lerp(baseColor, anisoSample, clamp(lod, 0.0f, 1.0f));
    }

    return anisoSample;
}

inline float4 ConeTrace(in Texture3D<float4> voxels, in float3 P, in float3 N, in float3 coneDirection, in float coneAperture, in bool traceOcclusion)
{
    float3 _color = 0;
    float _alpha = 0;

    traceOcclusion = traceOcclusion && voxel_radiance._aoAlpha < 1.0f;
    float _falloff = 0.5f * voxel_radiance._aoFalloff * voxel_radiance._dataSize;

    // �̹漺������ ���ø� �� �� ���� �� ���� ����ġ
    float3 _weight = coneDirection * coneDirection;

    float _occlusion = 0.0f;

    float4 _coneSam = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // ���� �������� ��ü�ȿ��� �ﰢ�� ������� ���� �л��Ű�� ����
    // 2 * sqrt2�� ���ø��� �������� ���� ���� ����� ������ ��� �� ���̵��� �ϴ� ������ �մϴ�.
    float3 _startPos = P + N * voxel_radiance._dataSize * 2 * sqrt2; // sqrt2 is diagonal voxel half-extent
    float _dist = voxel_radiance._dataSize; // �̹� ���� ��������
    
    int _currMip = -1;

    while (_dist < voxel_radiance._maxDistance && _coneSam.a < 1.f)
    {
        // ���� ��ġ���� �� ���� ������ �ٴڸ���� ������ ���� �ݰ� ���� ������ ũ�⺸�� �۴ٸ� ���� ũ���
        float _diameter = max(voxel_radiance._dataSize, 2 * coneAperture * _dist);

        // Cone�� �ٴڸ��� ������ ������ ���� ũ��� ���� ������ ����մϴ�.
        // ���� ���, diameter�� ���� ũ���� 2���� ���, mip ���� 1�� �˴ϴ�. �̴� �Ӹ� ���� 1�� �ǹ��մϴ�.
        // �ݸ�, diameter�� ���� ũ���� 4���� ���, mip ���� 2�� �˴ϴ�. �̴� �Ӹ� ���� 2�� �ǹ��մϴ�.
        float _mip = log2(_diameter * voxel_radiance._dataSizeRCP);

        // ���ø��� ������ ��ǥ�� ���Ѵ�.
        float3 _voxelPosition = _startPos + coneDirection * _dist;
        _voxelPosition = (_voxelPosition - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;
        _voxelPosition *= voxel_radiance._dataResRCP;
        _voxelPosition = _voxelPosition * float3(0.5f, -0.5f, 0.5f) + 0.5f; // dx������ y���� ���������

        // ���� ������ ���� ������ ���� ���ų� ���ø��� ���� ���� ������ ����
        if (any(_voxelPosition < 0) || any(_voxelPosition > 1) || _mip >= (float)voxel_radiance._mips)
            break;

        [branch]
        if (_currMip != _mip)
        {
            _currMip = _mip;
            float4 _anisoSample = AnistropicSample(_voxelPosition, _weight, coneDirection, _mip);

            _coneSam += (1.0f - _coneSam.a) * _anisoSample;

            [flatten]
            if (traceOcclusion && _occlusion < 1.0)
            {
                _occlusion += ((1.0f - _occlusion) * _anisoSample.a) / (1.0f + _falloff * _diameter);
            }
        }

        // ray�� ���� ��Ŵ diameter
        _dist += _diameter * voxel_radiance._rayStepSize;
    }

    return float4(_coneSam.rgb, _occlusion);
}

inline float4 ConeTraceRadiance(in Texture3D<float4> voxels, in float3 P, in float3 N, in bool ambientOcclusion)
{
    float4 radiance = 0;

    //float _aperture = PI / 3.0f; // Cone ���� 60��

    float _tanHalfAperture = 0.57735026919f;// tan(_aperture / 2.0f); // Cone�� �ظ��� ����
    float3 _guide = float3(0.0f, 1.0f, 0.0f);

    if (abs(dot(N, _guide)) == 1.0f)
    {
        _guide = float3(0.0f, 0.0f, 1.0f);
    }

    float3 _right = normalize(_guide - dot(N, _guide) * N);
    float3 _up = cross(_right, N);

    for (uint cone = 0; cone < voxel_radiance._numCones; ++cone)
    {
        float3 _coneDirection = N;
        _coneDirection += diffuseConeDirections[cone].x * _right + diffuseConeDirections[cone].z *
            _up;
        _coneDirection = normalize(_coneDirection);

        radiance += ConeTrace(voxels, P, N, _coneDirection, _tanHalfAperture, ambientOcclusion) * diffuseConeWeights[cone];
    }

    // ��� ConeTrace�� ������ Cone���� ��� ������ ����
    radiance.rgb *= voxel_radiance._numConesRCP;

    return max(0, radiance);
}

inline float4 ConeTraceReflection(in Texture3D<float4> voxels, in float3 P, in float3 N, in float3 V, in float roughness)
{
    float3 coneDirection = reflect(-V, N);

    float tanAperture = max(tan(HALF_PI * roughness * voxel_radiance.temp), 0.0174533f);

    float4 reflection = ConeTrace(voxels, P, N, coneDirection, tanAperture, false);

    return float4(max(0, reflection.rgb), saturate(reflection.a));
}

float4 InDirectLighting(in float3 P, in float3 N, in float3 albedo, in float roughness, in float metallic, bool ambientOcclusion)
{
    float4 _specularTrace = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 _diffuseTrace = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 _coneDirection = float3(0.0f, 0.0f, 0.0f);

    float3 V = normalize(camera._camWorld - P);

    _specularTrace = ConeTraceReflection(voxelTexture, P, N, V, roughness);

    if (any(albedo > _diffuseTrace.rgb))
    {
        _diffuseTrace = ConeTraceRadiance(voxelTexture, P, N, ambientOcclusion);

        _diffuseTrace.rgb *= albedo;
    }

    float3 result = voxel_radiance._inDirectFactor * (_diffuseTrace.rgb + _specularTrace.rgb);

    float3 R = reflect(-V, N);

    const float MAX_REFLECTION_LOD = 5.0;

    static const float kSpecularCoefficient = 0.04;

    float3 _specularColor = lerp(kSpecularCoefficient, albedo.xyz, metallic);

    float3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), _specularColor, roughness);
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    float2 envBRDF = gIntegrateBRDFMap.Sample(samWrapLinear, float2(max(dot(N, V), 0.0), roughness)).rg;
    float3 specular = _specularTrace.rgb * (F * envBRDF.x + envBRDF.y);

    float3 ambient = (kD * _diffuseTrace.rgb + _specularTrace.rgb) * _diffuseTrace.a;

    return float4(ambient, ambientOcclusion ? clamp(1.0f - _diffuseTrace.a + voxel_radiance._aoAlpha, 0.0f, 1.0f) : 1.0f);
}

float4 main(VSOutput input) : SV_TARGET
{
    //unpack gbuffer
   float4 _albedo = gAlbedo.Sample(samWrapLinear, input.uv);
   float4 _normal = gNormal.Sample(samWrapLinear, input.uv);
   float _depth = gDepth.Sample(samWrapLinear, input.uv);
   float4 _worldPos = gWorldPos.Sample(samWrapLinear, input.uv);
   float3 _viewPos = GetPositionVS(input.uv, _depth);

   float _roughness = lerp(0.04f, 1.0f, _normal.w);
   float _metallic = lerp(0.04f, 1.0f, _albedo.w);

   _albedo.w = 1.0f;
   _albedo = pow(_albedo, 2.2);

   float3 N = ((_normal - 0.5f) * 2.0f).xyz;

   float3 V = normalize(camera._camWorld - _worldPos.xyz);

   float3 _finColor = float3(0.0f, 0.0f, 0.0f);

   float4 _inDirectLight = float4(0.0f, 0.0f, 0.0f, 0.0f);

   float4 _directLight = float4(0.0f, 0.0f, 0.0f, 0.0f);

   [branch]
   if (voxel_radiance._mode == 0) // direct + indirect + ao
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, _metallic, true);
   }
   else if (voxel_radiance._mode == 1) // direct + indirect
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, _metallic, false);
   }
   else if (voxel_radiance._mode == 2) // direct
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
   }
   else if (voxel_radiance._mode == 3) // indirect
   {
       _directLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, _metallic, true);
   }
   else if (voxel_radiance._mode == 4) // ambient occlusion
   {
       _directLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, _metallic, true);
       _inDirectLight.rgb = float3(1.0f, 1.0f, 1.0f);
   }


   _finColor = _directLight.rgb + _inDirectLight.rgb;

   _finColor *= _inDirectLight.a;

   _finColor = pow(_finColor, 1 / 2.2);


   return float4(_finColor, 1.0f);
}