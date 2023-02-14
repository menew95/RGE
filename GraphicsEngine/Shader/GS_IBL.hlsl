cbuffer matirxs : register(b1)
{
	matrix g_View[6];
	matrix g_Proj;
}
struct GSInput
{
	float4 posW		: SV_POSITION;
};

struct StreamOutput
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	uint rtID : SV_RenderTargetArrayIndex;
};

[maxvertexcount(18)]
void main(triangle GSInput input[3], inout TriangleStream<StreamOutput> output)
{
	for (int view = 0; view < 6; view++)
	{
		StreamOutput _output;
		_output.rtID = view;

		for (int ver = 0; ver < 3; ver++)
		{
			_output.posH = mul(input[ver].posW, g_View[view]);
			_output.posH = mul(_output.posH, g_Proj);
			_output.posW = input[ver].posW;
			output.Append(_output);
		}

		output.RestartStrip();
	}
}