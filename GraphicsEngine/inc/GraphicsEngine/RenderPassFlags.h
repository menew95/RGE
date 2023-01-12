#pragma once

#include "GraphicsModule/Core/PipelineStateFlags.h"
#include "GraphicsModule/Core/PipelineLayoutFlags.h"
#include "GraphicsModule/Core/RenderTargetFlags.h"

namespace Graphics
{
	struct RenderPassDesc
	{
		GraphicsPipelineDesc	_graphicsPipelineDesc;
		PipelineLayoutDesc		_pipelineLayoutDesc;
		RenderTargetDesc		_renderTargetDesc;
	};
}