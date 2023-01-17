#include "Header\H_Input.hlsli"
#include "Header\H_ConstBuffer.hlsli"

Texture2D gAlbedoMap	: register(t0);

#if !defined(_NORMAL_MAP)
Texture2D gBumpMap		: register(t1);
#endif

SamplerState samWrapLinear	: register(s0);

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
void NormalSampleToWorldSpace(out float3 bumpedNormalW, in float3 normalMapSample, in float3 unitNormalW, in float3 tangentW)
{
	//bumpedNormalW = { 0.0f, 0.0f, 0.0f };
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);
	float3 BT = normalize(B - dot(B, N) * N - B * dot(B, T) * T);

	float3x3 TBN = float3x3(T, BT, N);

	// Transform from tangent space to world space.
	bumpedNormalW = mul(normalT, TBN);
}

PSOut main(VSOutput input)
{
	PSOut _out;

	_out.Albedo = gAlbedoMap.Sample(samWrapLinear, input.uv);

	float3 _normal = { 0.0f, 0.0f, 0.0f };

#if !defined(_NORMAL_MAP)
	_normal = input.normal;
#else
	float3 normalMapSample = gBumpMap.Sample(samWrapLinear, input.uv).rgb;
	
	float3 bumpedNormalW = { 0.0f, 0.0f, 0.0f };

	NormalSampleToWorldSpace(bumpedNormalW, normalMapSample, input.normal, input.tangent);

	_normal = bumpedNormalW;
#endif

	_out.Normal = float4((_normal * 0.5 + 0.5), material.roughness);

	_out.Depth = float4(input.posH.z, input.posH.z, input.posH.z, 1.0f);

	_out.World = float4(input.posW.xyz, 1.0f);

	_out.Emissive = float4(float3(material.emissive.rgb), input.posV.z);

	return _out;
}