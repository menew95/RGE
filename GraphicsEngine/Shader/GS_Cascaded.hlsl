#include "Header/H_Struct.hlsli"

cbuffer CascadedLight : register(b0)
{
	CascadedInfo _cascadedInfo;
}

struct GSInput
{
	float4 posW : SV_POSITION;
};

struct GSOutput
{
	float4 posH : SV_POSITION;
	uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(12)]
void main(triangle GSInput input[3], inout TriangleStream <GSOutput> output)
{
	for (int face = 0; face < 4; ++face)
	{
		GSOutput element;
		element.RTIndex = face;
		for (int i = 0; i < 3; ++i)
		{
			element.posH = mul(input[i].posW, _cascadedInfo._cascadedLightTM[face]);
			output.Append(element);
		}
		output.RestartStrip();
	}
}