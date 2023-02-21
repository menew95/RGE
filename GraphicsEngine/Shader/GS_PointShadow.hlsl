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

[maxvertexcount(18)]
void main(triangle GSInput input[3], inout TriangleStream<StreamOutput> output)
{
	for (uint cnt = 0; cnt < 1; cnt++)
	{
		for (uint view = 0; view < 6; view++)
		{
			StreamOutput _output;

			int _viewIdx = cnt * 6 + view;
			_output.rtID = _viewIdx;

			for (uint ver = 0; ver < 3; ver++)
			{
				// 랜더 오브젝트를 컬링해서 가지고 들어 올 수 없으니 삼각형 면을 컬링 하는게 성능상에 더 이득이 있는가?
				_output.posH = mul(input[ver].posW, _pointLight[cnt]._lightTransform[view]);
				output.Append(_output);
			}
			output.RestartStrip();
		}
	}
}