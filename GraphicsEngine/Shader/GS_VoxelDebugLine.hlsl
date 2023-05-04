#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Voxel.hlsli"
#include "Header/H_Util.hlsli"

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

[maxvertexcount(15)]
void main(point GSInput input[1], inout LineStream<GSOutput> output)
{
	[branch]
	if (input[0].color.a > 0)
	{
		float3 _pos = input[0].pos;

		_pos = _pos * voxel_radiance._dataResRCP * 2 - 1;
		_pos.y = -_pos.y;

		_pos *= voxel_radiance._dataRes;

		for (uint i = 0; i < 9; i++)
		{
			GSOutput _output;

			_output.color = float4(0, 1, 0, 1);

			_output.posH = float4(_pos + (CreateCubeLine(i) - float3(0, 1, 0)) * 2, 1.0f);

			_output.posH.xyz *= voxel_radiance._dataRes * voxel_radiance._dataSize / voxel_radiance._dataRes;
			_output.posH.xyz += voxel_radiance._gridCenter;

			_output.posH = mul(_output.posH, camera._view);
			_output.posH = mul(_output.posH, camera._proj);

			output.Append(_output);
		}

		output.RestartStrip();

		for (uint i2 = 9; i2 < 15; i2 += 2)
		{
			for (uint j = 0; j < 2; j++)
			{
				GSOutput _output;

				_output.color = float4(0, 1, 0, 1);

				_output.posH = float4(_pos+ (CreateCubeLine(i2 + j) - float3(0, 1, 0)) * 2, 1.0f);

				_output.posH.xyz *= voxel_radiance._dataRes * voxel_radiance._dataSize / voxel_radiance._dataRes;
				_output.posH.xyz += voxel_radiance._gridCenter;

				_output.posH = mul(_output.posH, camera._view);
				_output.posH = mul(_output.posH, camera._proj);

				output.Append(_output);
			}

			output.RestartStrip();
		}
	}
}