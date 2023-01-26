#include "Header/H_Struct.hlsli"

//#ifdef PBR_LIGHT
#include "Header/H_PBR.hlsli"
//#endif

#if !defined(H_LIGHT)

#define H_LIGHT

float CalcFallOff(float dist, float radius)
{
	float div = dist / radius;
	float q = div * div * div * div;
	float dis2 = div * div;
	float sat = saturate(1. - q);

	return sat * sat / (dis2 + 1.);
}

float3 Fresnel_Shlick(in float3 f0, in float3 f90, in float x)
{
	return f0 + (f90 - f0) * pow(1.f - x, 5.f);
}

float Diffuse_BurleyMSDN(in float NdotL, in float NdotV, in float LdotH, in float roughness)
{
	float fd90 = 0.5f + 2.f * roughness * LdotH * LdotH;
	return Fresnel_Shlick(1, fd90, NdotL).x * Fresnel_Shlick(1, fd90, NdotV).x;
}

float Specular_D_GGX(in float alpha, in float NdotH)
{
	const float alpha2 = alpha * alpha;
	const float lower = (NdotH * NdotH * (alpha2 - 1)) + 1;
	return alpha2 / max(EPSILON, PI * lower * lower);
}

float G_Shlick_Smith_Hable(float alpha, float LdotH)
{
	return rcp(lerp(LdotH * LdotH, 1, alpha * alpha * 0.25f));
}

float3 Specular_BRDF(in float alpha, in float3 specularColor, in float NdotV, in float NdotL, in float LdotH, in float NdotH)
{
	float specular_D = Specular_D_GGX(alpha, NdotH);

	float3 specular_F = Fresnel_Shlick(specularColor, 1, LdotH);

	float specular_G = G_Shlick_Smith_Hable(alpha, LdotH);

	return specular_D * specular_F * specular_G;
}

float3 CalcDirectionalLight(in Light light
	, in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 L = normalize(-light.Direction);

	float NdotV = saturate(dot(N, V));
	if (NdotV <= 0.0) return litColor;

	const float alpha = roughness * roughness;

	// half Vector
	float3 H = normalize(L + V);

	float NdotL = saturate(dot(N, L));
	float LdotH = saturate(dot(L, H));
	float NdotH = saturate(dot(N, H));

	float diffuse_factor = Diffuse_BurleyMSDN(NdotL, NdotV, LdotH, roughness);
	float3 specular = Specular_BRDF(alpha, specColor, NdotV, NdotL, LdotH, NdotH);

	//float3 brdf = CookTorrance_GGX(roughness2, metallic, specColor, diffColor, NoH, NoV, NoL, HoV);

	litColor += NdotL * light.Color * (((diffColor * diffuse_factor) + specular));

	return litColor;
}

float3 CalcPointLight(in Light light
	, in float3 position
	, in float roughness2
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVec = light.Position - position;
	float distance = length(lightVec);

	if (distance > light.Range)
		return litColor;

	float3 L = normalize(lightVec);

	float NoL = dot(N, L);
	if (NoL <= 0.0) return litColor;

	float NoV = dot(N, V);
	if (NoV <= 0.0) return litColor;

	float3 H = normalize(L + V);
	float NoH = dot(N, H);
	float HoV = dot(H, V);

	// Attenuation
	//float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.Range));
	//float attenuate = DistToLightNorm * DistToLightNorm;
	float attenuate = CalcFallOff(distance, light.Range);

	float3 radiance = attenuate * light.Power;

	float3 brdf = CookTorrance_GGX(roughness2, metallic, specColor, diffColor, NoH, NoV, NoL, HoV);

	litColor += light.Color * radiance * brdf;
	return litColor;
}

float3 CalcSpotLight(in Light light
	, in float3 position
	, in float roughness2
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 lightVec = normalize(light.Position - position);
	float distance = length(lightVec);

	if (distance > light.Range)
		return litColor;

	float3 L = normalize(lightVec);

	float NoL = dot(N, L);
	if (NoL <= 0.0) return litColor;

	float NoV = dot(N, V);
	if (NoV <= 0.0) return litColor;

	float3 H = normalize(L + V);
	float NoH = dot(N, H);
	float HoV = dot(H, V);

	float cosAng = dot(-light.Direction, L);

	float conAttenuate = 0.0f;

	if (cosAng < light.SpotAngle)
	{
		// cosAng이 더 작으면 사이각이 더 크다.
		conAttenuate = 0.0f;
		//return float3(1, 0, 0);
	}
	else if (cosAng > light.FallOffAngle)
	{
		// cosAng이 더 작고 FallOffAngle크면 감쇄가 시작되는 각도보다 작다.
		conAttenuate = 1.0f;
		//return float3(0, 1, 0);
	}
	else
	{
		//return float3(1, 1, 1);
		// 감쇄되는 영역 안에 있다.
		/*conAttenuate = saturate((light.SpotAngle - cosAng) / (light.SpotAngle - light.FallOffAngle));
		conAttenuate *= conAttenuate;*/
		conAttenuate = 1 - CalcFallOff(light.SpotAngle - cosAng, light.SpotAngle - light.FallOffAngle);
	}

	/*float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.Range));
	float attenuation = DistToLightNorm * DistToLightNorm;*/
	float attenuation = CalcFallOff(distance, light.Range);
	float3 radiance = attenuation * conAttenuate * light.Power;

	float3 brdf = CookTorrance_GGX(roughness2, metallic, specColor, diffColor, NoH, NoV, NoL, HoV);

	litColor += light.Color * radiance * brdf;

	return litColor;
}

float3 CalLight(in int lightCnt
	, in Light L[MAX_LIGHT_COUNT]
	, in float3 baseColor
	, in float3 position
	, in float3 N
	, in float3 V
	, in float roughness
	, in float metallic)
{
	float3 Lo = float3(0.0, 0.0, 0.0);

	float roughness2 = max(.001, roughness * roughness);

	float3 specularColor = baseColor * metallic;
	float3 diffuseColor = baseColor * (1 - metallic);


	float3 dir = normalize(L[0].Direction);

	for (int i = 0; i < lightCnt; i++)
	{
		switch (L[i].Type)
		{
			case 0:
			{
				Lo += CalcDirectionalLight(L[i], roughness, metallic, specularColor, diffuseColor, N, V);
				break;
			}
			case 1:
			{
				Lo += CalcPointLight(L[i], position, roughness2, metallic, specularColor, diffuseColor, N, V);
				break;
			}
			case 2:
			{
				Lo += CalcSpotLight(L[i], position, roughness2, metallic, specularColor, diffuseColor, N, V);
				break;
			}
		}
	}

	return Lo;
}

float3 CalLight(in Light L
	, in float3 specularColor
	, in float3 diffuseColor
	, in float3 position
	, in float3 N
	, in float3 V
	, in float roughness2
	, in float metallic)
{
	float3 Lo = float3(0.0, 0.0, 0.0);

	float3 dir = normalize(L.Direction);

	switch (L.Type)
	{
		case 0:
		{
			Lo += CalcSpotLight(L, position, roughness2, metallic, specularColor, diffuseColor, N, V);
			break;
		}
		case 1:
		{
			Lo += CalcDirectionalLight(L, roughness2, metallic, specularColor, diffuseColor, N, V);
			break;
		}
		case 2:
		{
			Lo += CalcPointLight(L, position, roughness2, metallic, specularColor, diffuseColor, N, V);
			break;
		}
	}

	return Lo;
}

#endif // H_LIGHT