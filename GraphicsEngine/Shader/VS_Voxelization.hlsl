#include "Header/H_ConstBuffer.hlsli"

struct VSInput
{
	float3	 posL			: POSITION;
	float4	 color			: COLOR;
	float2	 uv				: TEXCOORD;
	float3	 normal			: NORMAL;
	float3	tangent			: TANGENT;
	uint	bone[4]			: BONE;
	float	weight[4]		: WEIGHT;
};

struct VSOutput
{
    float4 posW		: POSITION;
    float4 color    : COLOR;
    float2 uv		: TEXCOORD;
    float3 normal	: NORMAL;

#if defined(_NORMAL_MAP)
	float3 tangent	: TANGENT;
#endif
};


VSOutput main(VSInput input)
{
	VSOutput _output;

#if !defined(_SKIN) && !defined(BONECNT)

	_output.posW = mul(float4(input.posL, 1.0f), world);
	_output.normal = normalize(mul(input.normal, (float3x3)worldInvTranspose));

#if defined(_NORMAL_MAP)
	_output.tangent = normalize(mul(input.tangent, (float3x3)world));
#endif // _NORMAL_MAP

#else // !_SKIN && !BONECNT

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
            _posL += input.weight[i] * mul(float4(input.posL, 1.0f), boneTransforms[input.bone[i]]).xyz;
            _normalL += input.weight[i] * mul(input.normal, (float3x3)boneTransforms[input.bone[i]]).xyz;

#if defined(_NORMAL_MAP)
            _tangentL += input.weight[i] * mul(input.tangent, (float3x3)boneTransforms[input.bone[i]]).xyz;
#endif //_NORMAL_MAP
        }
    }

    _output.posW = float4(_posL, 1.0f);

    _output.normal = normalize(_normalL);

#if defined(_NORMAL_MAP)
    _output.tangent = normalize(mul(_tangentL, (float3x3)world));
#endif // _NORMAL_MAP

#endif // !_SKIN && !BONECNT

    _output.uv = input.uv;
    _output.color = input.color;

	return _output;
}