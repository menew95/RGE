#if !defined(H_SHADOW)

#define H_SHADOW

static const int2 offset[9] =
{
	int2(-1, -1), int2(0, -1), int2(1, -1),
		int2(-1, 0), int2(0, 0), int2(1, 0),
		int2(-1, +1), int2(0, +1), int2(1, +1)
};

float CalcShadowFactor(SamplerComparisonState samShadow,
	Texture2D shadowMap,
	float3 shadowPosH)
{
	// Depth in NDC space.
	float depth = shadowPosH.z - 0.001f;

	float percentLit = 0.0f;

	shadowPosH.x = shadowPosH.x * 0.5f + 0.5f;
	shadowPosH.y = -shadowPosH.y * 0.5f + 0.5f;

	[unroll]
	for (int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy, depth, offset[i]).r;
	}
	return percentLit /= 9.0f;
}

float CalcCascadeShadowFactor(int cascadeIndex, float4 lightspacepos, Texture2DArray cascadeShadowMap, SamplerComparisonState cascadeShadowSampler)
{
	float3 projCoords = lightspacepos.xyz / lightspacepos.w;
	projCoords.x = projCoords.x * 0.5 + 0.5f;
	projCoords.y = -projCoords.y * 0.5 + 0.5f;
	if (projCoords.z > 1.0)
		return 0.0f;

	float currentDepth = projCoords.z;
	float bias = 0.01f;
	float shadow = 0.0;

	float3 samplePos = projCoords;
	samplePos.z = cascadeIndex;
	[unroll]
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			shadow += cascadeShadowMap.SampleCmpLevelZero(cascadeShadowSampler, samplePos, currentDepth - bias, int2(x, y));
		}
	}
	shadow /= 9.0f;
	return shadow;
}

#endif // H_SHADOW