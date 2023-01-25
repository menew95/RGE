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

float CalcShadowFactor2(SamplerState samShadow,
	Texture2D shadowMap,
	float3 shadowPosH)
{
	// Depth in NDC space.
	float depth = shadowPosH.z - 0.001;

	float percentLit = 0.0f;

	for (int i = 0; i < 9; ++i)
	{
		float temp = shadowMap.Sample(samShadow, shadowPosH.xy, offset[i]).r;

		if (depth <= temp)
		{
			percentLit += 1;
		}
		/*percentLit += shadowMap.SampleCmpLevelZero(samShadow,
			shadowPosH.xy, depth, offset[i]).r;*/
	}

	return percentLit /= 9.0f;
}

#endif // H_SHADOW