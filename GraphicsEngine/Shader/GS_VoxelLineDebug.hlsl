#include "Header/H_Voxel.hlsli"

struct GSInput
{
	float3 pos :POSITION;
	float4 color :COLOR;
};

struct GSOutput
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

[maxvertexcount(12)]
void main(point GSInput input[1], inout LineStream<GSOutput> output)
{
	[branch]
	if (input[0].color.a > 0)
	{
		for (uint i = 0; i < 12; i++)
		{
			GSOutput _output;

			_output.posH = float4(input[0].pos, 1.0f);
			_output.color = float4(1, 0, 0, 1); // input[0].color;



			output.Append(_output);
		}

		output.RestartStrip();
	}
}