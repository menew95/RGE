#include "Header\H_ConstBuffer.hlsli"


struct VSInput
{
    float3	 posL			: POSITION;
    float2	 uv				: TEXCOORD;
};

struct VertexOut
{
    //float4 pos : SV_POSITION;
    float4 posH : SV_POSITION;
    float3 viewRay : VIEWRAY;
    float2 uv : TEXCOORD;
};

VertexOut main(VSInput input)
{
    VertexOut _output;

    _output.posH = float4(input.posL, 1.0f);

    //_output.posH = input.uv * float2(1280, 720);

    float4 cameraRay = float4(input.uv * 2.0 - 1.0, 1.0, 1.0);

    // 다이렉트의 경우 좌상단이 0,0 이라 y값을 인버스 해줘야함
    cameraRay.y = -cameraRay.y;

    cameraRay = mul(cameraRay, camera._projInv);
    _output.viewRay = cameraRay;

    _output.uv = input.uv;

    return _output;
}