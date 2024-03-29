#include "Header/H_ConstBuffer.hlsli"
#include "Header/H_Voxel.hlsli"
#include "Header/H_Util.hlsli"

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

			// 복셀 텍스처 좌표계에서 복셀 공간 좌표계로
			_output.posH.xyz = _output.posH.xyz * voxel_radiance._dataResRCP * 2 - 1;
			_output.posH.y	= -_output.posH.y;
			
			
			_output.posH.xyz *= voxel_radiance._dataRes;
			
			// 복셀 공간의 정육면체의 각 모서리로 이동
			_output.posH.xyz += (CreateCube(i) - float3(0, 1, 0)) * 2;

			// 세계 공간 좌표계로 변경
			_output.posH.xyz *= voxel_radiance._dataRes * voxel_radiance._dataSize / voxel_radiance._dataRes;
			_output.posH.xyz += voxel_radiance._gridCenter;

			_output.posH = mul(_output.posH, camera._view);
			_output.posH = mul(_output.posH, camera._proj);

			output.Append(_output);
		}

		output.RestartStrip();
	}
}