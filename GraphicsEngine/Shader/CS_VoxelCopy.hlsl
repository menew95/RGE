#include "Header/H_Voxel.hlsli"

RWStructuredBuffer<VoxelType> VoxelGrid : register(u0);

RWTexture3D<float4> VoxelTexture : register(u1);

/*
    Buffer에 기록된 내용을 3D Texture에 복사
*/

[numthreads(64, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
   // uint id = DTid.x * 4;

   //for (uint i = 0; i < 4; i++)
   // {
   //     uint id0 = id;
   //     //uint id1 = id + 1;
   //     //uint id2 = id + 2;
   //     //uint id3 = id + 3;

   //     uint3 writecoord0 = Unflatten3D(id0, voxel_radiance._dataRes);
   //     //uint3 writecoord1 = Unflatten3D(id1, voxel_radiance._dataRes);
   //     //uint3 writecoord2 = Unflatten3D(id2, voxel_radiance._dataRes);
   //     //uint3 writecoord3 = Unflatten3D(id3, voxel_radiance._dataRes);

   //     uint _colorMask0 = VoxelGrid[id0]._colorMask;
   //     //uint _colorMask1 = VoxelGrid[id1]._colorMask;
   //     //uint _colorMask2 = VoxelGrid[id2]._colorMask;
   //     //uint _colorMask3 = VoxelGrid[id3]._colorMask;

   //     VoxelGrid[id0]._colorMask = 0;
   //     //VoxelGrid[id1]._colorMask = 0;
   //     //VoxelGrid[id2]._colorMask = 0;
   //     //VoxelGrid[id3]._colorMask = 0;

   //     float4 _colors0 = DecodeColor(_colorMask0);
   //     //float4 _colors1 = DecodeColor(_colorMask1);
   //     //float4 _colors2 = DecodeColor(_colorMask2);
   //     //float4 _colors3 = DecodeColor(_colorMask3);

   //     VoxelTexture[writecoord0] = _colors0;
   //     //VoxelTexture[writecoord1] = _colors1;
   //     //VoxelTexture[writecoord2] = _colors2;
   //     //VoxelTexture[writecoord3] = _colors3;
   // }

    uint id0 = DTid.x * 8;
    uint id1 = id0 + 1;
    uint id2 = id0 + 2;
    uint id3 = id0 + 3;

    uint3 writecoord0 = Unflatten3D(id0, voxel_radiance._dataRes);
    uint3 writecoord1 = Unflatten3D(id1, voxel_radiance._dataRes);
    uint3 writecoord2 = Unflatten3D(id2, voxel_radiance._dataRes);
    uint3 writecoord3 = Unflatten3D(id3, voxel_radiance._dataRes);

    uint _colorMask0 = VoxelGrid[id0]._colorMask;
    uint _colorMask1 = VoxelGrid[id1]._colorMask;
    uint _colorMask2 = VoxelGrid[id2]._colorMask;
    uint _colorMask3 = VoxelGrid[id3]._colorMask;

    VoxelGrid[id0]._colorMask = 0;
    VoxelGrid[id1]._colorMask = 0;
    VoxelGrid[id2]._colorMask = 0;
    VoxelGrid[id3]._colorMask = 0;

    float4 _colors0 = DecodeColor(_colorMask0);
    float4 _colors1 = DecodeColor(_colorMask1);
    float4 _colors2 = DecodeColor(_colorMask2);
    float4 _colors3 = DecodeColor(_colorMask3);

    VoxelTexture[writecoord0] = _colors0;
    VoxelTexture[writecoord1] = _colors1;
    VoxelTexture[writecoord2] = _colors2;
    VoxelTexture[writecoord3] = _colors3;
}

//[numthreads(256, 1, 1)]
//void main(uint3 DTid : SV_DispatchThreadID)
//{
//    uint3 writecoord = Unflatten3D(DTid.x, voxel_radiance._dataRes);
//
//    uint _colorMask = VoxelGrid[DTid.x]._colorMask;
//
//    float4 _colors = DecodeColor(_colorMask);
//
//    VoxelTexture[writecoord] = _colors;
//
//    VoxelGrid[DTid.x]._colorMask = 0;
//}

//[numthreads(4, 4, 4)]
//void main(uint3 DTid : SV_DispatchThreadID)
//{
//    /*uint3 _id = DTid * 2;
//
//    [unroll(8)]
//    for (uint i = 0; i < 8; i++)
//    {
//        uint3 _temp = uint3(0, 0, 0);
//
//        _temp.x = (i && 1u);
//        _temp.y = (i && 2u) >> 1;
//        _temp.z = (i && 4u) >> 2;
//
//        uint3 _wirtePos = _id + _temp;
//
//        uint _id = Flatten3D(_wirtePos, voxel_radiance._dataRes);
//
//        VoxelType voxel = VoxelGrid[_id];
//
//        float4 _color = DecodeColor(voxel._colorMask);
//
//        VoxelTexture[_wirtePos] = _color;
//
//        VoxelGrid[_id]._colorMask = 0;
//    }*/
//
//    uint3 _id = DTid * 2;
//
//    uint _idies[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//
//    uint3 _wirtePos[8] = {
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0),
//        uint3(0, 0, 0)
//    };
//
//    float4 _colors[8] = {
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f),
//        float4(0.f, 0.f, 0.f, 0.f)
//    };
//
//    _wirtePos[0] = _id + uint3(0, 0, 0);
//    _wirtePos[1] = _id + uint3(1, 0, 0);
//    _wirtePos[2] = _id + uint3(0, 0, 1);
//    _wirtePos[3] = _id + uint3(1, 0, 1);
//    _wirtePos[4] = _id + uint3(0, 1, 0);
//    _wirtePos[5] = _id + uint3(1, 1, 0);
//    _wirtePos[6] = _id + uint3(0, 1, 1);
//    _wirtePos[7] = _id + uint3(1, 1, 1);
//
//    _idies[0] = Flatten3D(_wirtePos[0], voxel_radiance._dataRes);
//    _idies[1] = Flatten3D(_wirtePos[1], voxel_radiance._dataRes);
//    _idies[2] = Flatten3D(_wirtePos[2], voxel_radiance._dataRes);
//    _idies[3] = Flatten3D(_wirtePos[3], voxel_radiance._dataRes);
//    _idies[4] = Flatten3D(_wirtePos[4], voxel_radiance._dataRes);
//    _idies[5] = Flatten3D(_wirtePos[5], voxel_radiance._dataRes);
//    _idies[6] = Flatten3D(_wirtePos[6], voxel_radiance._dataRes);
//    _idies[7] = Flatten3D(_wirtePos[7], voxel_radiance._dataRes);
//
//    VoxelType voxel[8] = {
//        VoxelGrid[_idies[0]],
//        VoxelGrid[_idies[1]],
//        VoxelGrid[_idies[2]],
//        VoxelGrid[_idies[3]],
//        VoxelGrid[_idies[4]],
//        VoxelGrid[_idies[5]],
//        VoxelGrid[_idies[6]],
//        VoxelGrid[_idies[7]]
//    };
//
//    _colors[0] = DecodeColor(voxel[0]._colorMask);
//    _colors[1] = DecodeColor(voxel[1]._colorMask);
//    _colors[2] = DecodeColor(voxel[2]._colorMask);
//    _colors[3] = DecodeColor(voxel[3]._colorMask);
//    _colors[4] = DecodeColor(voxel[4]._colorMask);
//    _colors[5] = DecodeColor(voxel[5]._colorMask);
//    _colors[6] = DecodeColor(voxel[6]._colorMask);
//    _colors[7] = DecodeColor(voxel[7]._colorMask);
//
//    VoxelTexture[_wirtePos[0]] = _colors[0];
//    VoxelTexture[_wirtePos[1]] = _colors[1];
//    VoxelTexture[_wirtePos[2]] = _colors[2];
//    VoxelTexture[_wirtePos[3]] = _colors[3];
//    VoxelTexture[_wirtePos[4]] = _colors[4];
//    VoxelTexture[_wirtePos[5]] = _colors[5];
//    VoxelTexture[_wirtePos[6]] = _colors[6];
//    VoxelTexture[_wirtePos[7]] = _colors[7];
//
//    VoxelGrid[_idies[0]]._colorMask = 0;
//    VoxelGrid[_idies[1]]._colorMask = 0;
//    VoxelGrid[_idies[2]]._colorMask = 0;
//    VoxelGrid[_idies[3]]._colorMask = 0;
//    VoxelGrid[_idies[4]]._colorMask = 0;
//    VoxelGrid[_idies[5]]._colorMask = 0;
//    VoxelGrid[_idies[6]]._colorMask = 0;
//    VoxelGrid[_idies[7]]._colorMask = 0;
//}