#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Voxel.hlsli"

struct GSInput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct GSOutput
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

inline float3 CreateCube(in uint vertexID)
{
	uint b = 1u << vertexID;
	return float3((0x287a & b) != 0, (0x02af & b) != 0, (0x31e3 & b) != 0);
}

[maxvertexcount(14)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> output)
{
	[branch]
	if (input[0].color.a > 0)
	{
		for (uint i = 0; i < 14; i++)
		{
			GSOutput _output;

			_output.posH = float4(input[0].pos, 1.0f);
			_output.color = input[0].color;

			_output.posH.xyz	= _output.posH.xyz * voxel_radiance._dataResRCP * 2 - 1;
			_output.posH.y	= -_output.posH.y;
			_output.posH.xyz *= voxel_radiance._dataRes;
			_output.posH.xyz += (CreateCube(i) - float3(0, 1, 0)) * 2;
			_output.posH.xyz *= voxel_radiance._dataRes * voxel_radiance._dataSize / voxel_radiance._dataRes;

			_output.posH.xyz += voxel_radiance._gridCenter;

			_output.posH = mul(_output.posH, camera._view);
			_output.posH = mul(_output.posH, camera._proj);

			output.Append(_output);
		}

		output.RestartStrip();
	}
}