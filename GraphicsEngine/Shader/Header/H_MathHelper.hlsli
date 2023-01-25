#if !defined(H_MATH_HELPER)

#define H_MATH_HELPER

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N) * N);
	float3 B = cross(N, T);
	float3 BT = normalize(B - dot(B, N) * N - B * dot(B, T) * T);

	float3x3 TBN = float3x3(T, BT, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
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