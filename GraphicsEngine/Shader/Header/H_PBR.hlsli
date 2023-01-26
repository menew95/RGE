#if !defined(H_PBR)
#define H_PBR

static const float PI = 3.14159265f;
static const float EPSILON = 1e-6f;

float Pow5(in float value)
{
	return value * value * value * value * value;
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * (1 / PI);
}

// [Burley 2012, "Physically-Based Shading at Disney"]
float3 Diffuse_Burley(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
	float FdV = 1 + (FD90 - 1) * pow(1 - NoV, 5);
	float FdL = 1 + (FD90 - 1) * pow(1 - NoL, 5);
	return DiffuseColor * ((1 / PI) * FdV * FdL);
}

// [Gotanda 2012, "Beyond a Simple Physically Based Blinn-Phong Model in Real-Time"]
float3 Diffuse_OrenNayar(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float a = Roughness * Roughness;
	float s = a;// / ( 1.29 + 0.5 * a );
	float s2 = s * s;
	float VoL = 2 * VoH * VoH - 1;		// double angle identity
	float Cosri = VoL - NoV * NoL;
	float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
	float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * (Cosri >= 0 ? rcp(max(NoL, NoV)) : 1);
	return DiffuseColor / PI * (C1 + C2) * (1 + Roughness * 0.5);
}

// [Gotanda 2014, "Designing Reflectance Models for New Consoles"]
float3 Diffuse_Gotanda(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float F0 = 0.04;
	float VoL = 2 * VoH * VoH - 1;		// double angle identity
	float Cosri = VoL - NoV * NoL;
#if 1
	float a2_13 = a2 + 1.36053;
	float Fr = (1 - (0.542026 * a2 + 0.303573 * a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996 * a2 * a + 1.50912 * a2 - 1.16402 * a) * pow(1 - NoV, 1 + rcp(39 * a2 * a2 + 1)) + 1);
	//float Fr = ( 1 - 0.36 * a ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( -2.5 * Roughness * ( 1 - NoV ) + 1 );
	float Lm = (max(1 - 2 * a, 0) * (1 - Pow5(1 - NoL)) + min(2 * a, 1)) * (1 - 0.5 * a * (NoL - 1)) * NoL;
	float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
	float Bp = Cosri < 0 ? 1.4 * NoV * NoL * Cosri : Cosri;
	float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
	return DiffuseColor / PI * Lr;
#else
	float a2_13 = a2 + 1.36053;
	float Fr = (1 - (0.542026 * a2 + 0.303573 * a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996 * a2 * a + 1.50912 * a2 - 1.16402 * a) * pow(1 - NoV, 1 + rcp(39 * a2 * a2 + 1)) + 1);
	float Lm = (max(1 - 2 * a, 0) * (1 - Pow5(1 - NoL)) + min(2 * a, 1)) * (1 - 0.5 * a + 0.5 * a * NoL);
	float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
	float Bp = Cosri < 0 ? 1.4 * NoV * Cosri : Cosri / max(NoL, 1e-8);
	float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
	return DiffuseColor / PI * Lr;
#endif
}

float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
	float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);

	float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;

	float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
	float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);

	return diffuseColor * lightScatter * viewScatter * energyFactor / PI;
}

float3 FresnelSchlick(float3 f0, float cosTheta)
{
	// Schlick
	// F( N, V ) = F_0 + (1 - F_0) * ( 1 - N °§ V) ^ 5
	// F_0 = ( (n_1 - n_2) / (n_1 + n_2) ) ^ 2 => IOR 
	float s = 1.0 - cosTheta;

	float powT = s * s * s * s * s;

	return f0 + (1.0 - f0) * saturate(powT);
}

// [Kelemen 2001, "A microfacet based coupled specular-matte brdf model with importance sampling"]
float Vis_Kelemen(float VoH)
{
	// constant to prevent NaN
	return rcp(4 * VoH * VoH + 1e-5);
}

float Vis_Schlick(float a2, float NoV, float NoL)
{
	float k = sqrt(a2) * 0.5;
	float Vis_SchlickV = NoV * (1 - k) + k;
	float Vis_SchlickL = NoL * (1 - k) + k;
	return 0.25 / (Vis_SchlickV * Vis_SchlickL);
}

float Vis_Schlick_Beckmann(float roughness, float NoV, float NoL)
{
	float k = (roughness + 1) * (roughness + 1) / 8;
	float Vis_SchlickV = NoV * (1 - k) + k;
	float Vis_SchlickL = NoL * (1 - k) + k;
	return 0.25 / (Vis_SchlickV * Vis_SchlickL);
}

// Smith term for GGX
// [Smith 1967, "Geometrical shadowing of a random rough surface"]
float Vis_Smith(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoV + sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoL + sqrt(NoL * (NoL - NoL * a2) + a2);
	return rcp(Vis_SmithV * Vis_SmithL);
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox(float a2, float NoV, float NoL)
{
	float a = sqrt(a2);
	float Vis_SmithV = NoL * (NoV * (1 - a) + a);
	float Vis_SmithL = NoV * (NoL * (1 - a) + a);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJoint(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoL * sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoV * sqrt(NoL * (NoL - NoL * a2) + a2);
	return 0.5f / max(Vis_SmithV + Vis_SmithL, 1e-5f);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

inline half SmithVisibilityTerm(half NdotL, half NdotV, half k)
{
	half gL = NdotL * (1 - k) + k;
	half gV = NdotV * (1 - k) + k;

	return 1.0 / (gL * gV + 1e-5f);
}

inline half SmitBeckmannVisbilityTerm(half NdotL, half NdotV, half roughness)
{
	half c = 0.797884560802865h;
	half k = roughness * c;
	return SmithVisibilityTerm(NdotL, NdotV, k) * 0.25f;
}

float GGX_Shlick(float ndotv, float k)
{
	return ndotv / (ndotv * (1 - k) + k);
}

float GGX_Smith_Geometry(float nv, float nl, float roughness)
{
	float r2 = roughness * roughness;
	float gv = nl + sqrt(nv * (nv - nv * r2) + r2);
	float gl = nv + sqrt(nl * (nl - nl * r2) + r2);
	return rcp(gv * gl);
}

float Neumann_Geometry(float cosThetaNL, float cosThetaNV)
{
	return cosThetaNL * cosThetaNV / max(cosThetaNL, cosThetaNV);
}

float Cook_Torrance_Geometry(float cosThetaNH, float cosThetaNV, float cosThetaVH, float cosThetaNL)
{
	float GV = 2 * cosThetaNH * cosThetaNV / cosThetaVH;
	float GL = 2 * cosThetaNH * cosThetaNL / cosThetaVH;
	return min(1, min(GV, GL));
}

float Kelemen_Geometry(float cosThetaNL, float cosThetaNV, float cosThetaVH)
{
	return cosThetaNL * cosThetaNV / (cosThetaVH * cosThetaVH);
}

float GGX_PartialGeometry(float cosThetaN, float alpha)
{
	float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
	float tan2 = (1.0f - cosTheta_sqr) / cosTheta_sqr;
	float GP = 2 / (1 + sqrt(1 + alpha * alpha * tan2));
	return GP;
}

float GGX_Distribution(float cosThetaNH, float alpha)
{
	float a2 = alpha * alpha;
	float d = (cosThetaNH * a2 - cosThetaNH) * cosThetaNH + 1.0;
	return a2 / (d * d * PI);
}

// ∫ÒµÓπÊº∫ GGX
// ax, x√‡ ∞≈ƒ•±‚
// ay, y√‡ ∞≈ƒ•±‚
// X ≈∫¡®∆Æ
// Y πŸ¿Ã≥Î∏÷
float GGX_Aniso_Distribution(float ax, float ay, float cosThetaNH, float3 H, float3 X, float3 Y)
{
	float cosThetaXH = dot(X, H);
	float cosThetaYH = dot(Y, H);
	float d = cosThetaXH * cosThetaXH / (ax * ax) + cosThetaYH * cosThetaYH / (ay * ay) + cosThetaNH * cosThetaNH;
	return 1 / (PI * ax * ay * d * d);
}

float Blinn_Phong_Distribution(float cosThetaNH, float alpha)
{
	float alpha2 = alpha * alpha;
	float pows = (2 / alpha2) - 2;
	float piAlpha2 = PI * alpha2;

	// (n°§m)^(2 / a^2 - 2) / (pi * a^2)
	return pow(cosThetaNH, pows) / piAlpha2;
}

float Beckmann_Distribution(float cosThetaNH, float alpha)
{
	float alpha2 = alpha * alpha;
	float piAlpha2 = PI * alpha2;
	float nh2 = cosThetaNH * cosThetaNH;
	float nh4 = nh2 * nh2;

	float pows = (nh2 - 1) / (alpha2 * nh2);

	// 1 / (pi * a^2 * (n°§m)^4) * e^(((n°§m)^2 - 1) / (a^2 * (n°§m)^2))
	return pow(EPSILON, pows) / (nh4 * piAlpha2);
}

float3 CookTorrance_GGX(in float roughness2
	, in float metallic
	, in float3 specColor
	, in float3 diffColor
	, in float NoH
	, in float NoV
	, in float NoL
	, in float HoV)
{
	float3 F = FresnelSchlick(specColor, HoV);
	float G = Vis_SmithJoint(roughness2, NoV, NoL);
	float D = GGX_Distribution(NoH, roughness2);

	float3 specK = F * G * D;

	float3 kD = 1 - F;
	kD *= 1 - metallic;

	float3 diffK = Diffuse_Lambert(kD * diffColor);

	return (diffK + specK) * NoL;
}

#endif // H_PBR