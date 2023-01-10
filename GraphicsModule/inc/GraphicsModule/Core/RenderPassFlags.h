#pragma once

namespace Graphics
{
	// render pass ����
	struct RenderPassDesc
	{
		class PipelineState* _pipelineState = nullptr;
		class PipelineLayout* _pipelineLayout = nullptr;

		class RenderTarget* _renderTarget = nullptr;
	};
}