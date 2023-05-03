#if !defined(H_MATH)

#define H_MATH

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

	normalize(bumpedNormalW);
}

//---------------------------------------------------------------------------------------
// CubeMap Sampler
//---------------------------------------------------------------------------------------
float4 CubeMapSampler(SamplerState sam, TextureCube cubeMap, float3 toEye, float3 normal)
{
	// CubeMap Reflection
	float3 incident = -toEye;
	float3 reflectionVector = reflect(incident, normal);
	float4 reflectionColor = cubeMap.Sample(sam, reflectionVector);

	return reflectionColor;
}

#endif