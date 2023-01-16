#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Input.hlsli"


VSOutput main(VSInput input)
{
    VSOutput _output;

#if !defined(_SKIN) && !defined(BONECNT)
    _output.posW = mul(float4(input.posL, 1.0f), world);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);
    
    _output.normal = normalize(mul(input.normal, (float3x3)worldInvTranspose));

#if defined(_NORMAL_MESH)
    _output.tangent = input.tangent;
#endif //_NORMAL_MESH

#else // !defined(_SKIN) && !defined(BONECNT)
    float3 _posL = float3(0.0f, 0.0f, 0.0f);
    float3 _normalL = float3(0.0f, 0.0f, 0.0f);
#if defined(_NORMAL_MESH)
    float3 _tangentL = float3(0.0f, 0.0f, 0.0f);
#endif //_NORMAL_MESH

    [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] > 0)
        {
            _posL        += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
            _normalL     += input.weight[i] * mul(input.normal, (float3x3)boneTransforms[input.bone[i]]).xyz;

#if defined(_NORMAL_MESH)
            _tangentL    += input.weight[i] * mul(input.tangent, (float3x3)boneTransforms[input.bone[i]]).xyz;
#endif //_NORMAL_MESH
        }
    }

    _output.posW = mul(float4(_posL, 1.0f), world);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);

    _output.normal = normalize(mul(_normalL, (float3x3)worldInvTranspose));

#if defined(_NORMAL_MESH)
    _output.Tangent = normalize(mul(_tangentL, (float3x3)world));
#endif //_NORMAL_MESH

#endif // !defined(_SKIN) && !defined(BONECNT)

    _output.color = input.color;

    _output.uv = input.uv;

    return _output;
}