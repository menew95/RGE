#include "Header/H_PBR.hlsli"

TextureCube gEnviromentMap	: register(t0);

SamplerState samWrapLinear	: register(s0);

cbuffer IBL : register(b1)
{
    float roughness;
}

struct GSOutput
{
    float4 posH : SV_POSITION;
    float4 posW : POSITION;
    uint rtID : SV_RenderTargetArrayIndex;
};

float4 main(GSOutput input) : SV_TARGET
{
    float3 N = normalize(input.posW.xyz);
    float3 R = N;
    float3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    float3 prefilteredColor = float3(0.0f, 0.0f, 0.0f);


    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, N, roughness);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            prefilteredColor += gEnviromentMap.Sample(samWrapLinear, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    return float4(prefilteredColor, 1.0);
}