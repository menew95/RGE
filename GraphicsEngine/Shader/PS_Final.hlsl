#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"

Texture2D gRenderTarget	: register(t0);

SamplerState samWrapLinear	: register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 _final = gRenderTarget.Sample(samWrapLinear, input.uv);

	return _final;
}