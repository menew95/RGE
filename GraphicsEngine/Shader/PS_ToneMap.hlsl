#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"

//#define Reinhard
#define Filmic
//#define Uncharted

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gLightBuffer : register(t0);

SamplerState samLinearClamp : register(s0);

float3 ReinhardToneMap(float3 color)
{
    color *= g_fHardExposure;  // Hardcoded Exposure Adjustment
    color = color / (1 + color);
    return pow(color, 1 / 2.2);
}

float3 FilmicToneMap(float3 color)
{
    color *= g_fHardExposure;  // Hardcoded Exposure Adjustment
    color = max(0, color - 0.004); // Filmic Curve
    return (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
}

float3 Uncharted2Kernel(float3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float3 Uncharted2ToneMap(float3 color)
{
    float W = 11.2;
    color *= g_fHardExposure;  // Hardcoded Exposure Adjustment

    float ExposureBias = 2.0f;
    float3 curr = Uncharted2Kernel(ExposureBias * color);

    float3 whiteScale = 1.0f / Uncharted2Kernel(W);
    color = curr * whiteScale;

    return pow(color, 1 / 2.2);
}

float4 main(VSOutput input) : SV_TARGET
{
    float3 _finColor = gLightBuffer.Sample(samLinearClamp, input.uv).rgb;

#ifdef Reinhard
    _finColor = ReinhardToneMap(_finColor);
#endif

#ifdef Filmic
    _finColor = FilmicToneMap(_finColor);
#endif

#ifdef Uncharted
    _finColor = Uncharted2ToneMap(_finColor);
#endif

    return float4(_finColor, 1.0f);
}