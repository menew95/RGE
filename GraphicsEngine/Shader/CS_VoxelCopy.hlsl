#include "Header/H_Voxel.hlsli"

RWStructuredBuffer<VoxelType> VoxelGrid : register(u0);

RWTexture3D<float4> VoxelTexture : register(u1);

/*
    Buffer에 기록된 내용을 3D Texture에 복사
*/

[numthreads(4, 4, 4)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    /*uint3 _id = DTid * 2;

    [unroll(8)]
    for (uint i = 0; i < 8; i++)
    {
        uint3 _temp = uint3(0, 0, 0);

        _temp.x = (i && 1u);
        _temp.y = (i && 2u) >> 1;
        _temp.z = (i && 4u) >> 2;

        uint3 _wirtePos = _id + _temp;

        uint _id = Flatten3D(_wirtePos, voxel_radiance._dataRes);

        VoxelType voxel = VoxelGrid[_id];

        float4 _color = DecodeColor(voxel._colorMask);

        VoxelTexture[_wirtePos] = _color;

        VoxelGrid[_id]._colorMask = 0;
    }*/

    uint3 _id = DTid * 2;

    uint _idies[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    uint3 _wirtePos[8] = {
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0),
        uint3(0, 0, 0)
    };

    float4 _colors[8] = {
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f),
        float4(0.f, 0.f, 0.f, 0.f)
    };

    _wirtePos[0] = _id + uint3(0, 0, 0);
    _wirtePos[1] = _id + uint3(1, 0, 0);
    _wirtePos[2] = _id + uint3(0, 0, 1);
    _wirtePos[3] = _id + uint3(1, 0, 1);
    _wirtePos[4] = _id + uint3(0, 1, 0);
    _wirtePos[5] = _id + uint3(1, 1, 0);
    _wirtePos[6] = _id + uint3(0, 1, 1);
    _wirtePos[7] = _id + uint3(1, 1, 1);

    _idies[0] = Flatten3D(_wirtePos[0], voxel_radiance._dataRes);
    _idies[1] = Flatten3D(_wirtePos[1], voxel_radiance._dataRes);
    _idies[2] = Flatten3D(_wirtePos[2], voxel_radiance._dataRes);
    _idies[3] = Flatten3D(_wirtePos[3], voxel_radiance._dataRes);
    _idies[4] = Flatten3D(_wirtePos[4], voxel_radiance._dataRes);
    _idies[5] = Flatten3D(_wirtePos[5], voxel_radiance._dataRes);
    _idies[6] = Flatten3D(_wirtePos[6], voxel_radiance._dataRes);
    _idies[7] = Flatten3D(_wirtePos[7], voxel_radiance._dataRes);

    VoxelType voxel[8] = {
        VoxelGrid[_idies[0]],
        VoxelGrid[_idies[1]],
        VoxelGrid[_idies[2]],
        VoxelGrid[_idies[3]],
        VoxelGrid[_idies[4]],
        VoxelGrid[_idies[5]],
        VoxelGrid[_idies[6]],
        VoxelGrid[_idies[7]]
    };

    _colors[0] = DecodeColor(voxel[0]._colorMask);
    _colors[1] = DecodeColor(voxel[1]._colorMask);
    _colors[2] = DecodeColor(voxel[2]._colorMask);
    _colors[3] = DecodeColor(voxel[3]._colorMask);
    _colors[4] = DecodeColor(voxel[4]._colorMask);
    _colors[5] = DecodeColor(voxel[5]._colorMask);
    _colors[6] = DecodeColor(voxel[6]._colorMask);
    _colors[7] = DecodeColor(voxel[7]._colorMask);

    VoxelTexture[_wirtePos[0]] = _colors[0];
    VoxelTexture[_wirtePos[1]] = _colors[1];
    VoxelTexture[_wirtePos[2]] = _colors[2];
    VoxelTexture[_wirtePos[3]] = _colors[3];
    VoxelTexture[_wirtePos[4]] = _colors[4];
    VoxelTexture[_wirtePos[5]] = _colors[5];
    VoxelTexture[_wirtePos[6]] = _colors[6];
    VoxelTexture[_wirtePos[7]] = _colors[7];

    VoxelGrid[_idies[0]]._colorMask = 0;
    VoxelGrid[_idies[1]]._colorMask = 0;
    VoxelGrid[_idies[2]]._colorMask = 0;
    VoxelGrid[_idies[3]]._colorMask = 0;
    VoxelGrid[_idies[4]]._colorMask = 0;
    VoxelGrid[_idies[5]]._colorMask = 0;
    VoxelGrid[_idies[6]]._colorMask = 0;
    VoxelGrid[_idies[7]]._colorMask = 0;
}