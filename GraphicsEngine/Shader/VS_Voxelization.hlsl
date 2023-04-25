#include "Header/H_ConstBuffer.hlsli"

struct VSInput
{
	float3	 posL			: POSITION;
	float4	 color			: COLOR;
	float2	 uv				: TEXCOORD;
	float3	 normal			: NORMAL;
	float3	tangent			: TANGENT;
	uint	bone[4]			: BONE;
	float	weight[4]		: WEIGHT;
};

struct VSOutput
{
    float4 posW		: POSITION;
    float2 uv		: TEXCOORD;
    float3 normalWS	: NORMAL;
};


VSOutput main(VSInput input)
{
	VSOutput _output;

	_output.posW = mul(float4(input.posL, 1.0f), world);
	_output.uv = input.uv;
	_output.normalWS = normalize(mul(input.normal, (float3x3)worldInvTranspose));

	return _output;
}