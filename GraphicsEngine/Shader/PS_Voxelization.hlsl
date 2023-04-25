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

cbuffer VoxelCbuf : register(b2)
{
    VoxelRadiance voxel_radiance;
}

void main(PSInput input)
{
    float3 _diff = (input.positionWS.xyz - voxel_radiance._gridCenter) * voxel_radiance._dataResRCP * voxel_radiance._dataSizeRCP;
    float3 _uv = _diff * float3(0.5f, -0.5f, 0.5f) + 0.5f;

    [branch]
    if (any(0 > _uv || 1 <= _uv))
    {
        return;
    }

    float3 _normal = normalize(input.normalWS);

    input.uv.y = 1 - input.uv.y;
    float4 _color = gAlbedoMap.Sample(samWrapLinear, input.uv);


    uint _colorEncoded = EncodeColor(_color);
    uint _normalEncoded = EncodeNormal(_normal);

    uint3 _writecoord = floor(_uv * voxel_radiance._dataRes);
    uint _id = Flatten3D(_writecoord, voxel_radiance._dataRes);

    InterlockedMax(VoxelGrid[_id]._colorMask, _colorEncoded);
    InterlockedMax(VoxelGrid[_id]._normalMask, _normalEncoded);
}