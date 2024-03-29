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

	[branch]
	if (NdotV > 0.0)
	{
		const float alpha = roughness * roughness;

		// half Vector
		float3 H = normalize(L + V);

		float NdotL = saturate(dot(N, L));
		float LdotH = saturate(dot(L, H));
		float NdotH = saturate(dot(N, H));

		float diffuse_factor = Diffuse_BurleyMSDN(NdotL, NdotV, LdotH, roughness);
		float3 specular = Specular_BRDF(alpha, specColor, NdotV, NdotL, LdotH, NdotH);

		litColor += NdotL * light._color * (((diffColor * diffuse_factor) + specular)) * light._power;

	}
	
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

	float3 L = normalize(lightVec);

	float NdotV = saturate(dot(N, V));

	[branch]
	if (distance <= light._range && NdotV < 0.0)
	{
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
	}

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

	float3 lightVec = light._position - position;
	float distance = length(lightVec);

	float NdotV = saturate(dot(N, V));

	[branch]
	if (distance <= light._range && NdotV > 0.0f)
	{
		float3 lightVec = light._position - position;
		float distance = length(lightVec);

		float3 L = normalize(lightVec);

		// L이 픽셀의 월드로부터 라이트의 월드임으로 빛의 방향을 뒤집어서 코사인
		float cosAng = dot(-light._direction, L);

		float conAttenuate = 0.0f;

		float innerCos = cos(light._fallOffAngle);
		float outCos = cos(light._spotAngle);

		[flatten]
		if (cosAng < outCos)
		{
			// cosAng이 더 작으면 사이각이 더 크다.
			conAttenuate = 0.0f;
		}
		else if (cosAng > innerCos)
		{
			// cosAng이 더 작고 FallOffAngle크면 감쇄가 시작되는 각도보다 작다.
			conAttenuate = 1.0f;
		}
		else
		{
			// 감쇄되는 영역 안에 있다.
			float angleRangeInv = 1.f / max(innerCos - outCos, 0.0001f);
			float angleRangeInv2 = -outCos * angleRangeInv;

			// 0도에서 90도 까지의.. Spot Attenuation (위의 코드와 다를바가 거의 없다)
			float spotAttn = pow(saturate(cosAng * angleRangeInv + angleRangeInv2), 2.0f);

			conAttenuate = spotAttn;// CalcFallOff(light._spotAngle - , light._spotAngle - light._fallOffAngle);
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
	}

	return litColor;
}

#endif // H_LIGHT