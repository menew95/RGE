#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Input.hlsli"

Texture2D gAlbedo	: register(t0);
Texture2D gNormal	: register(t1);
Texture2D gDepth	: register(t2);
Texture2D gWorldPos	: register(t3);
Texture2D gEmissive	: register(t4);

SamplerState samWrapLinear	: register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 _litColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	return _litColor;
}