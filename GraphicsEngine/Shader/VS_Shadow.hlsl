#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Input.hlsli"

struct Output
{
    float4 pos		: SV_POSITION;
};

Output main(VSInput input)
{
    Output _output;

#if defined(_USE_GS)

#if !defined(_SKIN) && !defined(BONECNT)
    _output.pos = mul(float4(input.posL, 1.0f), world);

#else // !defined(_SKIN) && !defined(BONECNT)
    float3 _posL = float3(0.0f, 0.0f, 0.0f);

    [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            _posL += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
        }
    }

    _output.pos = float4(_posL, 1.0f);

#endif // !defined(_SKIN) && !defined(BONECNT)

#else //defined(_USE_GS)

#if !defined(_SKIN) && !defined(BONECNT)
    _output.pos = mul(float4(input.posL, 1.0f), world);
    _output.pos = mul(_output.pos, camera._view);
    _output.pos = mul(_output.pos, camera._proj);

#else // !defined(_SKIN) && !defined(BONECNT)
    float3 _posL = float3(0.0f, 0.0f, 0.0f);

    [unroll]
    for (int i = 0; i < BONECNT; i++)
    {
        if (input.bone[i] >= 0)
        {
            _posL += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
        }
    }

    _output.pos = float4(_posL, 1.0f);
    _output.pos = mul(_output.pos, camera._view);
    _output.pos = mul(_output.pos, camera._proj);

#endif // !defined(_SKIN) && !defined(BONECNT)

#endif // defined(_USE_GS)

    return _output;
}