#pragma once

namespace Graphics
{
	// render pass ����
	struct RenderPassDesc
	{
		// state, shader bind
		class PipelineState* _pipelineState = nullptr;

		// resource bind
		class PipelineLayout* _pipelineLayout = nullptr;

		// rt, dsv bind
		class RenderTarget* _renderTarget = nullptr;
	};
}