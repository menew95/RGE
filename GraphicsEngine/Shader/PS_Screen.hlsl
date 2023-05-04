#include "Header/H_Input.hlsli"

SamplerState samWrapLinear : register(s0);

Texture2D g_Texture : register(t0);

float4 main(VSOutput input) : SV_Target
{
	float4 _color = g_Texture.Sample(samWrapLinear, input.uv);

	clip((_color.r < 0.001f && _color.g < 0.001f && _color.b < 0.001f && _color.a < 0.001f) ? -1 : 1 );

	return _color;
}