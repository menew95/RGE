#include "Header/H_Input.hlsli"

SamplerState samWrapLinear : register(s0);

Texture2D g_Texture : register(t0);

float4 main(VSOutput input) : SV_Target
{
	return g_Texture.Sample(samWrapLinear, input.uv);
}