#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Input.hlsli"

#define INSTANCING
#define _SKIN

#if defined(INSTANCING)


//struct InstanceData
//{
//    matrix world;
//    matrix worldInv;
//
//    skinning
//    matrix bonetrancform[BONE_MAX_CNT]
// 
//    Material _material
//};

Buffer<float4> g_instancingBuffer : register(t0);

inline void GetInstanceData(out matrix world, out matrix worldInv, in uint offset)
{
    world._11_12_13_14 = g_instancingBuffer[offset + 2];
    world._21_22_23_24 = g_instancingBuffer[offset + 3];
    world._31_32_33_34 = g_instancingBuffer[offset + 4];
    world._41_42_43_44 = g_instancingBuffer[offset + 5];

    worldInv._11_12_13_14 = g_instancingBuffer[offset + 6];
    worldInv._21_22_23_24 = g_instancingBuffer[offset + 7];
    worldInv._31_32_33_34 = g_instancingBuffer[offset + 8];
    worldInv._41_42_43_44 = g_instancingBuffer[offset + 9];
}

inline void GetInstanceBoneData(out matrix boneTransform, in uint boneOffset)
{
    boneTransform._11_12_13_14 = g_instancingBuffer[boneOffset];
    boneTransform._21_22_23_24 = g_instancingBuffer[boneOffset + 1];
    boneTransform._31_32_33_34 = g_instancingBuffer[boneOffset + 2];
    boneTransform._41_42_43_44 = g_instancingBuffer[boneOffset + 3];
}

VSOutput main(VSInput input, uint instanceID : SV_InstanceID)
{
    VSOutput _output;

#if defined(_SCREEN)
    _output.posH = float4(input.posL.xy, 1.0f, 1.0f);
    _output.normal = float4(1.0f, 1.0f, 1.0f, 1.0f);
#else

#if !defined(_SKIN) && !defined(BONECNT)

    //bufferStride = 13;
    uint _offset = instanceID * 13u;

    matrix _world, _worldInv;

    GetInstanceData(_world, _worldInv, _offset);

    _output.posW = mul(float4(input.posL, 1.0f), _world);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);

    _output.normal = normalize(mul(input.normal, (float3x3)_worldInv));

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(input.tangent, (float3x3)_world));
#endif //_NORMAL_MAP

#else // !defined(_SKIN) && !defined(BONECNT)

    uint _offset = instanceID * 525;

    float3 _posL = float3(0.0f, 0.0f, 0.0f);
    float3 _normalL = float3(0.0f, 0.0f, 0.0f);

#if defined(_NORMAL_MAP)

    float3 _tangentL = float3(0.0f, 0.0f, 0.0f);

#endif //_NORMAL_MAP

    [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            int _boneTransformStartOffset = _offset + 10;
            int _boneTransformOffset = _boneTransformStartOffset + input.bone[i] * 4;

            matrix _boneTransform;

            GetInstanceBoneData(_boneTransform, _boneTransformOffset);

            _posL += input.weight[i] * mul(float4(input.posL, 1.0f), _boneTransform).xyz;
            _normalL += input.weight[i] * mul(input.normal, (float3x3)_boneTransform).xyz;

#if defined(_NORMAL_MAP)
            _tangentL += input.weight[i] * mul(input.tangent, (float3x3)_boneTransform).xyz;
#endif //_NORMAL_MAP
        }
    }

    _output.posW = float4(_posL, 1.0f);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);

    _output.normal = normalize(_normalL);

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(_tangentL, (float3x3)world));
#endif //_NORMAL_MAP

#endif // !defined(_SKIN) && !defined(BONECNT)

    _output.color = input.color;

#endif // defined(_SCREEN)

    _output.uv = input.uv;

    return _output;
}

#else
VSOutput main(VSInput input)
{
    VSOutput _output;

#if defined(_SCREEN)
    _output.posH = float4(input.posL.xy, 1.0f, 1.0f);
    _output.normal = float4(1.0f, 1.0f, 1.0f, 1.0f);
#else

#if !defined(_SKIN) && !defined(BONECNT)
    _output.posW = mul(float4(input.posL, 1.0f), world);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);
    
    _output.normal = normalize(mul(input.normal, (float3x3)worldInvTranspose));

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(input.tangent, (float3x3)world));
#endif //_NORMAL_MAP

#else // !defined(_SKIN) && !defined(BONECNT)
    float3 _posL = float3(0.0f, 0.0f, 0.0f);
    float3 _normalL = float3(0.0f, 0.0f, 0.0f);

#if defined(_NORMAL_MAP)
    float3 _tangentL = float3(0.0f, 0.0f, 0.0f);

#endif //_NORMAL_MAP

    [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            _posL        += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
            _normalL     += input.weight[i] * mul(input.normal, (float3x3)boneTransforms[input.bone[i]]).xyz;

#if defined(_NORMAL_MAP)
            _tangentL    += input.weight[i] * mul(input.tangent, (float3x3)boneTransforms[input.bone[i]]).xyz;
#endif //_NORMAL_MAP
        }
    }

    _output.posW = float4(_posL, 1.0f);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);

    _output.normal = normalize(_normalL);

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(_tangentL, (float3x3)world));
#endif //_NORMAL_MAP

#endif // !defined(_SKIN) && !defined(BONECNT)

    _output.color = input.color;

#endif // defined(_SCREEN)

    _output.uv = input.uv;

    return _output;
}
#endif