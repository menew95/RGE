#include "Header/H_Voxel.hlsli"
#include "Header/H_ConstBuffer.hlsli"

RWStructuredBuffer<VoxelType> VoxelGrid : register(u0);

RWTexture3D<float4> voxelTexture : register(u1);

RWTexture3D<float4> secondBounceTexture : register(u2);

Texture3D<float4> voxelTexMip[6] : register(t0);


[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    VoxelType voxel = VoxelGrid[DTid.x];

    float4 color = DecodeColor(voxel._colorMask);

    if (color.a < EPSILON) return;

    uint3 writecoord = Unflatten3D(DTid.x, voxel_radiance._dataRes);
}