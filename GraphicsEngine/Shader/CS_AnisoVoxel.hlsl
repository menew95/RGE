#include "Header/H_Voxel.hlsli"

//static uint3 g_anisoOffsets[] =
//{
//	uint3(1, 1, 1),
//	uint3(1, 1, 0),
//	uint3(1, 0, 1),
//	uint3(1, 0, 0),
//	uint3(0, 1, 1),
//	uint3(0, 1, 0),
//	uint3(0, 0, 1),
//	uint3(0, 0, 0)
//};

static int3 g_anisoOffsets[8] =
{
	int3(-1, -1, -1),
	int3(-1, -1, 0),
	int3(-1, 0, -1),
	int3(-1, 0, 0),
	int3(0, -1, -1),
	int3(0, -1, 0),
	int3(0, 0, -1),
	int3(0, 0, 0)
};

SamplerState samWrapPoint : register(s0);

RWTexture3D<float4> VoxelTexture : register(u0);

RWTexture3D<float4> VoxelMipMap[6] : register(u1);

void FetchVoxels(in uint3 pos, inout float4 val[8])
{
	for (int i = 0; i < 8; i++)
	{
		val[i] = VoxelTexture[pos + g_anisoOffsets[i]];
	}
}

[numthreads(8, 8, 8)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint3 _writecoord = Unflatten3D(DTid.x, voxel_radiance._dataRes / 2);

	uint _mipDimension = voxel_radiance._dataRes / 2u;

	if (DTid.x >= _mipDimension ||
		DTid.y >= _mipDimension ||
		DTid.z >= _mipDimension)
		return;

	float4 values[8] =
	{
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f),
		float4(0.0f,0.0f,0.0f,0.0f)
	};

	uint3 sourcePos = _writecoord * 2;

	FetchVoxels(sourcePos, values);

	VoxelMipMap[0][_writecoord] =
		(
			values[0] + values[4] * (1 - values[0].a) +
			values[1] + values[5] * (1 - values[1].a) +
			values[2] + values[6] * (1 - values[2].a) +
			values[3] + values[7] * (1 - values[3].a)
			) * 0.25f;

	VoxelMipMap[1][_writecoord] =
		(
			values[4] + values[0] * (1 - values[4].a) +
			values[5] + values[1] * (1 - values[5].a) +
			values[6] + values[2] * (1 - values[6].a) +
			values[7] + values[3] * (1 - values[7].a)
			) * 0.25f;

	VoxelMipMap[2][_writecoord] =
		(
			values[0] + values[2] * (1 - values[0].a) +
			values[1] + values[3] * (1 - values[1].a) +
			values[5] + values[7] * (1 - values[5].a) +
			values[4] + values[6] * (1 - values[4].a)
			) * 0.25f;

	VoxelMipMap[3][_writecoord] =
		(
			values[2] + values[0] * (1 - values[2].a) +
			values[3] + values[1] * (1 - values[3].a) +
			values[7] + values[5] * (1 - values[7].a) +
			values[6] + values[4] * (1 - values[6].a)
			) * 0.25f;

	VoxelMipMap[4][_writecoord] =
		(
			values[0] + values[1] * (1 - values[0].a) +
			values[2] + values[3] * (1 - values[2].a) +
			values[4] + values[5] * (1 - values[4].a) +
			values[6] + values[7] * (1 - values[6].a)
			) * 0.25f;

	VoxelMipMap[5][_writecoord] =
		(
			values[1] + values[0] * (1 - values[1].a) +
			values[3] + values[2] * (1 - values[3].a) +
			values[5] + values[4] * (1 - values[5].a) +
			values[7] + values[6] * (1 - values[7].a)
			) * 0.25f;
}