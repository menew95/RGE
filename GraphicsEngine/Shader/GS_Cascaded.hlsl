#include "Header/H_Struct.hlsli"
cbuffer CascadedLight : register(b0)
{
	CascadedLight _cascadedLight;
}

struct GSInput
{
	float4 posW : SV_POSITION;
};

struct StreamOutput
{
	float4 posH : SV_POSITION;
	uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(12)]
void main(triangle GSInput input[3], inout TriangleStream <StreamOutput> output)
{
	for (int face = 0; face < 4; ++face)
	{
		StreamOutput _output;
		_output.RTIndex = face;
		for (int i = 0; i < 3; ++i)
		{
			_output.posH = mul(input[i].posW, _cascadedLight._lightTransform[face]);
			output.Append(_output);
		}
		output.RestartStrip();
	}
}