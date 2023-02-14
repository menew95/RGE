struct VSInput
{
	float3 posL		: POSITION;
};

struct VSOutput
{
	float4 posW		: SV_POSITION;
};

VSOutput main(VSInput input)
{
	VSOutput _output;
	_output.posW = float4(input.posL, 1.0f);
	return _output;
}