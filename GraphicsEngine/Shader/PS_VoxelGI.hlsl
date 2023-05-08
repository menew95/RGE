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

    // We need to offset the cone start position to avoid sampling its own voxel (self-occlusion):
    //	Unfortunately, it will result in disconnection between nearby surfaces :(
    float dist = voxel_radiance._dataSize; // offset by cone dir so that first sample of all cones are not the same
    float3 startPos = P + N * voxel_radiance._dataSize * 2 * sqrt2; // sqrt2 is diagonal voxel half-extent

    // We will break off the loop if the sampling distance is too far for performance reasons:
    while (dist < voxel_radiance._maxDistance && alpha < 1)
    {
        float diameter = max(voxel_radiance._dataSize, 2 * coneAperture * dist);
        float mip = log2(diameter * voxel_radiance._dataSizeRCP);

        // Because we do the ray-marching in world space, we need to remap into 3d texture space before sampling:
        //	todo: optimization could be doing ray-marching in texture space
        float3 tc = startPos + coneDirection * dist;
        tc = (tc - voxel_radiance._gridCenter) * voxel_radiance._dataSizeRCP;
        tc *= voxel_radiance._dataResRCP;
        tc = tc * float3(0.5f, -0.5f, 0.5f) + 0.5f;

        // break if the ray exits the voxel grid, or we sample from the last mip:
        if (any(tc < 0) || any(tc > 1) || mip >= (float)voxel_radiance._mips)
            break;

        float4 sam = voxels.SampleLevel(samClampLinear, tc, mip);

        // this is the correct blending to avoid black-staircase artifact (ray stepped front-to back, so blend front to back):
        float a = 1 - alpha;
        color += a * sam.rgb;
        alpha += a * sam.a;

        // step along ray:
        dist += diameter * voxel_radiance._rayStepSize;
    }

    return float4(color, alpha);
}
inline float4 ConeTraceRadiance(in Texture3D<float4> voxels, in float3 P, in float3 N)
{
    float4 radiance = 0;

    for (uint cone = 0; cone < voxel_radiance._numCones; ++cone) // quality is between 1 and 16 cones
    {
        // approximate a hemisphere from random points inside a sphere:
        //  (and modulate cone with surface normal, no banding this way)
        float3 coneDirection = normalize(CONES[cone] + N);
        // if point on sphere is facing below normal (so it's located on bottom hemisphere), put it on the opposite hemisphere instead:
        coneDirection *= dot(coneDirection, N) < 0 ? -1 : 1;

        radiance += ConeTrace(voxels, P, N, coneDirection, tan(PI * 0.5f * 0.33f));
    }

    // final radiance is average of all the cones radiances
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