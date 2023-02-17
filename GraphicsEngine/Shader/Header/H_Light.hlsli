#include "Header/H_Struct.hlsli"
#include "Header/H_PBR.hlsli"
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

float3 CalcDirectionalLight(in DirectionLight light
	, in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 L = normalize(-light._direction);

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

	litColor += NdotL * light._color * (((diffColor * diffuse_factor) + specular)) * light._power;

	return litColor;
}

float3 CalcPointLight(in PointLight light
	, in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V
	, in float3 position)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	float3 lightVec = light._position - position;
	float distance = length(lightVec);

	if (distance > light._range)
		return litColor;

	float3 L = normalize(lightVec);

	float NdotV = saturate(dot(N, V));
	if (NdotV <= 0.0) return litColor;

	const float alpha = roughness * roughness;

	// half Vector
	float3 H = normalize(L + V);

	float NdotL = saturate(dot(N, L));
	float LdotH = saturate(dot(L, H));
	float NdotH = saturate(dot(N, H));

	// Attenuation
	float attenuate = CalcFallOff(distance, light._range);

	float3 radiance = attenuate * light._power;

	float diffuse_factor = Diffuse_BurleyMSDN(NdotL, NdotV, LdotH, roughness);
	float3 specular = Specular_BRDF(alpha, specColor, NdotV, NdotL, LdotH, NdotH);

	litColor += NdotL * light._color * (((diffColor * diffuse_factor) + specular)) * radiance;

	return litColor;
}

float3 CalcSpotLight(in SpotLight light
	, in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float3 N
	, in float3 V
	, in float3 position)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	float3 lightVec = normalize(light._position - position);
	float distance = length(lightVec);

	if (distance > light._range)
		return litColor;

	float3 L = normalize(lightVec);

	float NdotV = saturate(dot(N, V));
	if (NdotV <= 0.0) return litColor;

	float cosAng = dot(-light._direction, L);

	float conAttenuate = 0.0f;

	if (cosAng < light._spotAngle)
	{
		// cosAng�� �� ������ ���̰��� �� ũ��.
		conAttenuate = 0.0f;
	}
	else if (cosAng > light._fallOffAngle)
	{
		// cosAng�� �� �۰� FallOffAngleũ�� ���Ⱑ ���۵Ǵ� �������� �۴�.
		conAttenuate = 1.0f;
	}
	else
	{
		// ����Ǵ� ���� �ȿ� �ִ�.
		conAttenuate = 1 - CalcFallOff(light._spotAngle - cosAng, light._spotAngle - light._fallOffAngle);
	}


	const float alpha = roughness * roughness;

	// half Vector
	float3 H = normalize(L + V);

	float NdotL = saturate(dot(N, L));
	float LdotH = saturate(dot(L, H));
	float NdotH = saturate(dot(N, H));

	// Attenuation
	float attenuate = CalcFallOff(distance, light._range);

	float3 radiance = attenuate * conAttenuate * light._power;

	float diffuse_factor = Diffuse_BurleyMSDN(NdotL, NdotV, LdotH, roughness);
	float3 specular = Specular_BRDF(alpha, specColor, NdotV, NdotL, LdotH, NdotH);

	litColor += NdotL * light._color * (((diffColor * diffuse_factor) + specular)) * radiance;

	return litColor;
}

#endif // H_LIGHT