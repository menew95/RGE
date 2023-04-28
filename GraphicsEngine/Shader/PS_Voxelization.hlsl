#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Voxel.hlsli"

Texture2D gAlbedoMap	: register(t0);

#if defined(_NORMAL_MAP)
Texture2D gBumpMap		: register(t1);
#endif

#if defined(_MRA_MAP)
Texture2D gMRAMap		: register(t2);
#endif

SamplerState samWrapLinear	: register(s0);

struct PSInput
{
    float4 position     : SV_POSITION;
    float4 positionWS   : POSITION;
    float2 uv           : TEXCOORD;
    float3 normalWS     : NORMAL;
};

RWStructuredBuffer<VoxelType> VoxelGrid     : register(u0);

void main(PSInput input)
{
    float3 _diff = (input.positionWS.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataResRCP * voxel_radiance._dataSizeRCP;
    float3 _voxelUVW = _diff * float3(0.5f, -0.5f, 0.5f) + 0.5f; // 복셀 텍스처 좌표계로 변환

    [branch]
    if (any(0 > _voxelUVW || 1 <= _voxelUVW))
    {
        return;
    }

    float3 _normal = normalize(input.normalWS);
    float4 _color = gAlbedoMap.Sample(samWrapLinear, input.uv);

    uint _colorEncoded = EncodeColor(_color);
    uint _normalEncoded = EncodeNormal(_normal);

    uint3 _writecoord = floor(_voxelUVW * voxel_radiance._dataRes);
    uint _id = Flatten3D(_writecoord, voxel_radiance._dataRes);

    InterlockedMax(VoxelGrid[_id]._colorMask, _colorEncoded);
    InterlockedMax(VoxelGrid[_id]._normalMask, _normalEncoded);
}