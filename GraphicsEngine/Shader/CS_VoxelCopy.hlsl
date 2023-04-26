#include "Header/H_Voxel.hlsli"

RWStructuredBuffer<VoxelType> VoxelGrid : register(u0);

RWTexture3D<float4> VoxelTexture : register(u1);

/*
    Buffer에 기록된 내용을 3D Texture에 복사
*/

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    VoxelType voxel = VoxelGrid[DTid.x];

    float4 color = DecodeColor(voxel._colorMask);

    uint3 writecoord = Unflatten3D(DTid.x, voxel_radiance._dataRes);

    [branch]
    if (color.a > 0)
    {
        VoxelTexture[writecoord] = float4(color.rgb, 1);
    }
    else
    {
        VoxelTexture[writecoord] = 0;
    }

    VoxelGrid[DTid.x]._colorMask = 0;

}