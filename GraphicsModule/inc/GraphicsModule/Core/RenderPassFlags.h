#pragma once

namespace Graphics
{
	// render pass ¼³°è
	struct RenderPassDesc
	{
		class PipelineState* _pipelineState = nullptr;
		class PipelineLayout* _pipelineLayout = nullptr;

		class RenderTarget* _renderTarget = nullptr;
	};
}