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

float4 main(GSOutput input) : SV_Target
{
    float3 normal = normalize(input.posW.xyz);

    float3 irradiance = float3(0.0f, 0.0f, 0.0f);

    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += gEnviromentMap.Sample(samWrapLinear, sampleVec).rgb * cos(theta)* sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.0f);
}