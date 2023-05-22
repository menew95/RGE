#include "Header/H_Voxel.hlsli"
#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"
#include "Header/H_Light.hlsli"
#include "Header/H_Shadow.hlsli"
#include "Header/H_Math.hlsli"

static const float3 CONES[] =
{
	float3(0.57735, 0.57735, 0.57735),
	float3(0.57735, -0.57735, -0.57735),
	float3(-0.57735, 0.57735, -0.57735),
	float3(-0.57735, -0.57735, 0.57735),
	float3(-0.903007, -0.182696, -0.388844),
	float3(-0.903007, 0.182696, 0.388844),
	float3(0.903007, -0.182696, 0.388844),
	float3(0.903007, 0.182696, -0.388844),
	float3(-0.388844, -0.903007, -0.182696),
	float3(0.388844, -0.903007, 0.182696),
	float3(0.388844, 0.903007, -0.182696),
	float3(-0.388844, 0.903007, 0.182696),
	float3(-0.182696, -0.388844, -0.903007),
	float3(0.182696, 0.388844, -0.903007),
	float3(-0.182696, 0.388844, 0.903007),
	float3(0.182696, -0.388844, 0.903007)
};

static const float sqrt2 = 1.414213562;
//static const float aoAlpha = 0.01f;
//static const float aoFalloff = 725.0f;
Texture2D gAlbedo	: register(t0);
Texture2D gNormal   : register(t1);
Texture2D<float> gDepth    : register(t2);
Texture2D gWorldPos	: register(t3);
Texture3D<float4> voxelTexture : register(t4);
Texture3D<float4> voxelTexMipmap[6] : register(t5);
Texture2D<float4> gDirectionLight : register(t11);

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
    
    // We will break off the loop if the sampling distance is too far for performance reasons:
    while (_dist < voxel_radiance._maxDistance && _alpha < 1)
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

        float4 _anisoSample = AnistropicSample(_voxelPosition, _weight, coneDirection, _mip);

        _coneSam += (1.0f - _coneSam.a) * _anisoSample;

        [flatten]
        if (traceOcclusion && _occlusion < 1.0)
        {
            _occlusion += ((1.0f - _occlusion) * _anisoSample.a) / (1.0f + _falloff * _diameter);
        }

        // ray�� ���� ��Ŵ diameter
        _dist += _diameter * voxel_radiance._rayStepSize;
    }

    return float4(_coneSam.rgb, _occlusion);
}

inline float4 ConeTraceRadiance(in Texture3D<float4> voxels, in float3 P, in float3 N)
{
    float4 radiance = 0;

    float aperture = PI / 3.0f; // Cone ���� 60��
    float tanHalfAperture = tan(aperture / 2.0f); // Cone�� �ظ��� ����

    for (uint cone = 0; cone < voxel_radiance._numCones; ++cone)
    {
        // �̸� ���ص� CONES ������ coneDirection�� ����
        //float3 coneDirection = normalize(CONES[cone] + N);
        float3 coneDirection = reflect(N, CONES[cone]);
        
        // Cone�� ������ ǥ������ �ݱ��ȿ� ������ ���� ����
        coneDirection *= dot(coneDirection, N) < 0 ? -1 : 1;

        radiance += ConeTrace(voxels, P, N, coneDirection, tanHalfAperture, true);
    }

    // ��� ConeTrace�� ������ Cone���� ��� ������ ����
    radiance.rgb *= voxel_radiance._numConesRCP;
    //radiance.a = saturate(radiance.a);

    return max(0, radiance);
}

inline float4 ConeTraceReflection(in Texture3D<float4> voxels, in float3 P, in float3 N, in float3 V, in float roughness)
{
    float3 coneDirection = reflect(-V, N);

    float tanAperture = clamp(tan(PI * 0.5f * roughness), 0.0174533f, PI);

    float4 reflection = ConeTrace(voxels, P, N, coneDirection, tanAperture, false);

    return float4(max(0, reflection.rgb), saturate(reflection.a));
}

float4 InDirectLighting(in float3 P, in float3 N, in float3 albedo, in float roughness, bool ambientOcclusion)
{
    float4 _specularTrace = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 _diffuseTrace = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 _coneDirection = float3(0.0f, 0.0f, 0.0f);

    float3 V = normalize(camera._camWorld - P);

    _specularTrace = ConeTraceReflection(voxelTexture, P, N, V, roughness);

    if (any(albedo > _diffuseTrace.rgb))
    {
        _diffuseTrace = ConeTraceRadiance(voxelTexture, P, N);

        _diffuseTrace.rgb *= albedo;
    }

    float3 result = voxel_radiance._inDirectFactor * (_diffuseTrace.rgb + _specularTrace.rgb);

    return float4(result, ambientOcclusion ? clamp(1.0f - _diffuseTrace.a + voxel_radiance._aoAlpha, 0.0f, 1.0f) : 1.0f);
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

   if (voxel_radiance._mode == 0) // direct + indirect + ao
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, true);
   }
   else if (voxel_radiance._mode == 1) // direct + indirect
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, false);
   }
   else if (voxel_radiance._mode == 2) // direct
   {
       _directLight = gDirectionLight.Sample(samWrapLinear, input.uv);
       _inDirectLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
   }
   else if (voxel_radiance._mode == 3) // indirect
   {
       _directLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, true);
   }
   else if (voxel_radiance._mode == 4) // ambient occlusion
   {
       _directLight = float4(0.0f, 0.0f, 0.0f, 1.0f);
       _inDirectLight = InDirectLighting(_worldPos.xyz, N, _albedo.xyz, _roughness, true);
       _inDirectLight.rgb = float3(1.0f, 1.0f, 1.0f);
   }


   _finColor = _directLight.rgb + _inDirectLight.rgb;

   _finColor *= _inDirectLight.a;

   _finColor = pow(_finColor, 1 / 2.2);


   return float4(_finColor, 1.0f);
}