#define _MAX_LIGHT_COUNT 3

#include "Header/H_ConstBuffer.hlsli"

struct GSInput
{
	float4 posW		: SV_POSITION;
};

struct StreamOutput
{
	float4 posH : SV_POSITION;
	uint rtID : SV_RenderTargetArrayIndex;
};

[maxvertexcount(_MAX_LIGHT_COUNT)]
void main(triangle GSInput input[3], inout TriangleStream<StreamOutput> output)
{
	for (uint cnt = 0; cnt < _lightCount.z; cnt++)
	{
		StreamOutput _output;

		_output.rtID = cnt;

		for (uint ver = 0; ver < 3; ver++)
		{
			// ���� ������Ʈ�� �ø��ؼ� ������ ��� �� �� ������ �ﰢ�� ���� �ø� �ϴ°� ���ɻ� �� �̵��� �ִ°�?
			_output.posH = mul(input[ver].posW, _spotLight[cnt]._lightTransform);
			output.Append(_output);
		}

		output.RestartStrip();
	}
}