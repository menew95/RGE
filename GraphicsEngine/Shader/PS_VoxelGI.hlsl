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

Texture2D gAlbedo	: register(t0);
Texture2D gNormal   : register(t1);
Texture2D<float> gDepth    : register(t2);
Texture2D gWorldPos	: register(t3);
Texture3D<float4> voxelTexture : register(t4);

SamplerState samWrapLinear	: register(s0);
SamplerState samClampLinear	: register(s1);

inline float4 ConeTrace(in Texture3D<float4> voxels, in float3 P, in float3 N, in float3 coneDirection, in float coneAperture)
{
    float3 color = 0;
    float alpha = 0;

    // ���� �������� ��ü�ȿ��� �ﰢ�� ������� ���� �л��Ű�� ����
    // 2 * sqrt2�� ���ø��� �������� ���� ���� ����� ������ ��� �� ���̵��� �ϴ� ������ �մϴ�.
    float3 startPos = P + N * voxel_radiance._dataSize * 2 * sqrt2; // sqrt2 is diagonal voxel half-extent
    float dist = voxel_radiance._dataSize; // �̹� ���� ��������
    
    // We will break off the loop if the sampling distance is too far for performance reasons:
    while (dist < voxel_radiance._maxDistance && alpha < 1)
    {
        // ���� ��ġ���� �� ���� ������ �ٴڸ���� ������ ���� �ݰ� ���� ������ ũ�⺸�� �۴ٸ� ���� ũ���
        float diameter = max(voxel_radiance._dataSize, 2 * coneAperture * dist);

        // Cone�� �ٴڸ��� ������ ������ ���� ũ��� ���� ������ ����մϴ�.
        // ���� ���, diameter�� ���� ũ���� 2���� ���, mip ���� 1�� �˴ϴ�. �̴� �Ӹ� ���� 1�� �ǹ��մϴ�.
        // �ݸ�, diameter�� ���� ũ���� 4���� ���, mip ���� 2�� �˴ϴ�. �̴� �Ӹ� ���� 2�� �ǹ��մϴ�.
        float mip = log2(diameter * voxel_radiance._dataSizeRCP);

        // ���ø��� ������ ��ǥ�� ���Ѵ�.
        float3 _voxelPosition = startPos + coneDirection * dist;
        _voxelPosition = (_voxelPosition - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;
        _voxelPosition *= voxel_radiance._dataResRCP;
        _voxelPosition = _voxelPosition * float3(0.5f, -0.5f, 0.5f) + 0.5f; // dx������ y���� ���������

        // ���� ������ ���� ������ ���� ���ų� ���ø��� ���� ���� ������ ����
        if (any(_voxelPosition < 0) || any(_voxelPosition > 1) || mip >= (float)voxel_radiance._mips)
            break;

        float4 sam = voxels.SampleLevel(samClampLinear, _voxelPosition, mip);

        // this is the correct blending to avoid black-staircase artifact (ray stepped front-to back, so blend front to back):
        // front to back blending�� ����
        float a = 1 - alpha;
        color += a * sam.rgb;
        alpha += a * sam.a;

        // step along ray:
        // ray�� ���� ��Ŵ diameter
        dist += diameter * voxel_radiance._rayStepSize;
    }

    return float4(color, alpha);
}
inline float4 ConeTraceRadiance(in Texture3D<float4> voxels, in float3 P, in float3 N)
{
    float4 radiance = 0;


    float aperture = PI / 3.0f; // Cone ���� 60��
    float tanHalfAperture = tan(aperture / 2.0f); // Cone�� �ظ��� ����

    for (uint cone = 0; cone < voxel_radiance._numCones; ++cone) // quality is between 1 and 16 cones
    {
        // approximate a hemisphere from random points inside a sphere:
        //  (and modulate cone with surface normal, no banding this way)
        
        // �̸� ���ص� CONES ������ coneDirection�� ����
        //float3 coneDirection = normalize(CONES[cone] + N);
        float3 coneDirection = reflect(N, CONES[cone]);
        
        // Cone�� ������ ǥ������ �ݱ��ȿ� ������ ���� ����
        coneDirection *= dot(coneDirection, N) < 0 ? -1 : 1;

        radiance += ConeTrace(voxels, P, N, coneDirection, tanHalfAperture);
    }

    // ��� ConeTrace�� ������ Cone���� ��� ������ ����
    radiance *= voxel_radiance._numConesRCP;
    radiance.a = saturate(radiance.a);

    return max(0, radiance);
}
inline float4 ConeTraceReflection(in Texture3D<float4> voxels, in float3 P, in float3 N, in float3 V, in float roughness)
{
    float aperture = tan(roughness * PI * 0.5f * 0.1f);
    float3 coneDirection = reflect(-V, N);

    float4 reflection = ConeTrace(voxels, P, N, coneDirection, aperture);

    return float4(max(0, reflection.rgb), saturate(reflection.a));
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

   float3 N = ((_normal - 0.5f) * 2.0f).xyz;

   return ConeTraceRadiance(voxelTexture, _worldPos.xyz, N);

}