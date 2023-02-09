#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Input.hlsli"


VSOutput main(VSInput input)
{
    VSOutput _output;

#if defined(_SCREEN)
    _output.posH = float4(input.posL.xy, 0.5f, 1.0f);
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

    //_output.posW = mul(float4(_posL, 1.0f), world);
    _output.posW = float4(_posL, 1.0f);
    _output.posV = mul(_output.posW, camera._view);
    _output.posH = mul(_output.posV, camera._proj);

    _output.normal = normalize(_normalL);// normalize(mul(_normalL, (float3x3)worldInvTranspose));

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(_tangentL, (float3x3)world));
#endif //_NORMAL_MAP

#endif // !defined(_SKIN) && !defined(BONECNT)

    _output.color = input.color;

#endif // defined(_SCREEN)

    _output.uv = input.uv;

    return _output;
}