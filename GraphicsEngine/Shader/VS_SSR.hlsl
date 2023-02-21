#include "Header\H_ConstBuffer.hlsli"


struct VSInput
{
    float3	 posL			: POSITION;
    float2	 uv				: TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 posH : POSITION;
    float3 viewRay : VIEWRAY;
    float2 uv : TEXCOORD;
};

VertexOut main(VSInput input)
{
    VertexOut _output;

    _output.pos = float4(input.posL, 1.0f);

    _output.posH = input.uv * float2(1280, 720);

    float4 cameraRay = float4(input.uv * 2.0 - 1.0, 1.0, 1.0);
    cameraRay = mul(cameraRay, camera._projInv);
    _output.viewRay = cameraRay.xyz / cameraRay.w;

    _output.uv = input.uv;

    return _output;
}