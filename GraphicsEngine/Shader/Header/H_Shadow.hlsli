#if !defined(H_SHADOW)

#define H_SHADOW

static const int2 offset[9] =
{
	int2(-1, -1), int2(0, -1), int2(1, -1),
		int2(-1, 0), int2(0, 0), int2(1, 0),
		int2(-1, +1), int2(0, +1), int2(1, +1)
};

static const float3 sampleOffsetDirections[20] =
{
	float3(1, 1, 1), float3(1, -1, 1), float3(-1, -1, 1), float3(-1, 1, 1),
	float3(1, 1, -1), float3(1, -1, -1), float3(-1, -1, -1), float3(-1, 1, -1),
	float3(1, 1, 0), float3(1, -1, 0), float3(-1, -1, 0), float3(-1, 1, 0),
	float3(1, 0, 1), float3(-1, 0, 1), float3(1, 0, -1), float3(-1, 0, -1),
	float3(0, 1, 1), float3(0, -1, 1), float3(0, -1, -1), float3(0, 1, -1)
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

uint SelectView(float3 position, float3 lightPos)
{
	// +x, -x, +y, -y, +z, -z
	float3 posToLight = position - lightPos;
	float xA = abs(posToLight.x);
	float yA = abs(posToLight.y);
	float zA = abs(posToLight.z);

	if (xA < yA)
	{
		if (yA < zA)
		{
			if (posToLight.z < 0) return 5; // -z
			else return 4; // +z
		}
		else
		{
			if (posToLight.y < 0) return 3; // -y
			else return 2; // -y
		}
	}
	else
	{
		if (xA < zA)
		{
			if (posToLight.z < 0) return 5; // -z
			else return 4; // +z
		}
		else
		{
			if (posToLight.x < 0) return 1; // -x
			else return 0; // -x
		}
	}
}

float CalcDepthInShadow(const in float3 fragPos, float far_plane, float near_plane)
{
	const float c1 = far_plane / (far_plane - near_plane);
	const float c0 = -near_plane * far_plane / (far_plane - near_plane);
	const float3 m = abs(fragPos).xyz;
	const float major = max(m.x, max(m.y, m.z));
	return (c1 * major + c0) / major;
}

float PointShadowFactor(int pointIndex, float4 lightspacepos, float3 posToLight, float far_plane, TextureCubeArray pointShadowMap, /*SamplerState shadowSampler*/ SamplerComparisonState shadowSampler)
{
	float3 projCoords = lightspacepos.xyz / lightspacepos.w;
	projCoords.x = projCoords.x * 0.5 + 0.5f;
	projCoords.y = -projCoords.y * 0.5 + 0.5f;
	if (projCoords.z > 1.0)
		return 0.0f; 

	//float currentDepth = CalcDepthInShadow(posToLight, far_plane, 0.1f);
	float currentDepth = projCoords.z;
	float bias = 0.01f;
	float shadow = 0.0;

	float width, height, element;
	pointShadowMap.GetDimensions(width, height, element);
	float textureSize = 1.0f / width;
	posToLight = normalize(posToLight);

	float4 samplePos = float4(posToLight, 0.0f);
	samplePos.a = pointIndex;

	int samples = 20;
	//float viewDistance = length(viewPos - worldPos);
	//float diskRadius = 1.0f / 512.f;
	//float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	//float offset = 0.1;

	[unroll]
	for (int i = 0; i < samples; ++i)
	{
		float4 _test = samplePos + float4(sampleOffsetDirections[i] * textureSize, 0.0f);

		//float closestDepth = pointShadowMap.SampleLevel(shadowSampler, _test, currentDepth);
		shadow += pointShadowMap.SampleCmpLevelZero(shadowSampler, samplePos, currentDepth);

		//closestDepth *= far_plane;   // undo mapping [0;1]
		/*if (currentDepth <= closestDepth)
			shadow += 1.0;*/
	}
	shadow /= float(samples);

	//for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	//{
	//	for (float y = -offset; y < offset; y += offset / (samples * 0.5))
	//	{
	//		for (float z = -offset; z < offset; z += offset / (samples * 0.5))
	//		{
	//			float closestDepth = pointShadowMap.SampleCmpLevelZero(shadowSampler, samplePos, currentDepth);
	//			//shadow += pointShadowMap.SampleCmpLevelZero(shadowSampler, samplePos, currentDepth - bias);
	//			//closestDepth *= far_plane;   // undo mapping [0;1]
	//			if (currentDepth - bias > closestDepth)
	//				shadow += 1.0;
	//		}
	//	}
	//}

	//shadow /= (samples * samples * samples);

	return shadow;
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