#include "Header/H_Voxel.hlsli"

static uint3 g_anisoOffsets[] =
{
	uint3(1, 0, 1),	// +++
	uint3(1, 0, 0),	// ++-
	uint3(1, 1, 1),	// +-+
	uint3(1, 1, 0),	// +--
	uint3(0, 0, 1),	// -++
	uint3(0, 0, 0),	// -+-
	uint3(0, 1, 1),	// --+
	uint3(0, 1, 0)	// ---
};

/*static int3 g_anisoOffsets[8] =
{
	int3(-1, -1, -1),
	int3(-1, -1, 0),
	int3(-1, 0, -1),
	int3(-1, 0, 0),
	int3(0, -1, -1),
	int3(0, -1, 0),
	int3(0, 0, -1),
	int3(0, 0, 0)
};*/

SamplerState samWrapPoint : register(s0);

RWTexture3D<float4> VoxelMipSrc[6] : register(u0);
RWTexture3D<float4> VoxelMipDst[6] : register(u6);

cbuffer VoxelCB :register(b6)
{
	uint _mipDimension;
}

void FetchVoxels(in uint3 pos, in int dir, inout float4 val[8])
{
	for (int i = 0; i < 8; i++)
	{
		val[i] = VoxelMipSrc[dir][pos + g_anisoOffsets[i]];
	}
}

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
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

	uint3 sourcePos = DTid * 2;

	FetchVoxels(sourcePos, 0, values);

	VoxelMipDst[0][DTid] =
		(
			values[0] + values[4] * (1 - values[0].a) +
			values[1] + values[5] * (1 - values[1].a) +
			values[2] + values[6] * (1 - values[2].a) +
			values[3] + values[7] * (1 - values[3].a)
			) * 0.25f;

	FetchVoxels(sourcePos, 1, values);

	VoxelMipDst[1][DTid] =
		(
			values[4] + values[0] * (1 - values[4].a) +
			values[5] + values[1] * (1 - values[5].a) +
			values[6] + values[2] * (1 - values[6].a) +
			values[7] + values[3] * (1 - values[7].a)
			) * 0.25f;

	FetchVoxels(sourcePos, 2, values);

	VoxelMipDst[2][DTid] =
		(
			values[0] + values[2] * (1 - values[0].a) +
			values[1] + values[3] * (1 - values[1].a) +
			values[5] + values[7] * (1 - values[5].a) +
			values[4] + values[6] * (1 - values[4].a)
			) * 0.25f;

	FetchVoxels(sourcePos, 3, values);

	VoxelMipDst[3][DTid] =
		(
			values[2] + values[0] * (1 - values[2].a) +
			values[3] + values[1] * (1 - values[3].a) +
			values[7] + values[5] * (1 - values[7].a) +
			values[6] + values[4] * (1 - values[6].a)
			) * 0.25f;

	FetchVoxels(sourcePos, 4, values);

	VoxelMipDst[4][DTid] =
		(
			values[0] + values[1] * (1 - values[0].a) +
			values[2] + values[3] * (1 - values[2].a) +
			values[4] + values[5] * (1 - values[4].a) +
			values[6] + values[7] * (1 - values[6].a)
			) * 0.25f;


	FetchVoxels(sourcePos, 5, values);

	VoxelMipDst[5][DTid] =
		(
			values[1] + values[0] * (1 - values[1].a) +
			values[3] + values[2] * (1 - values[3].a) +
			values[5] + values[4] * (1 - values[5].a) +
			values[7] + values[6] * (1 - values[7].a)
			) * 0.25f;
}