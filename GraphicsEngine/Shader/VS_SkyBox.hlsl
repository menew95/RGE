#include "Header/H_ConstBuffer.hlsli"

struct VSInput
{
	float3 posL		: POSITION;
};

struct VSOutput
{
	float4 posH		: SV_POSITION;
	float3 worldPos	: POSITION;
};

// vertex shader 
VSOutput main(VSInput input)
{
	VSOutput _output;

	_output.posH = mul(float4(input.posL, 1.0f), camera._worldViewProj).xyzw;
	_output.worldPos = input.posL;

	return _output;
}