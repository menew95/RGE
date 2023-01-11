#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11RenderPass.h"

namespace Graphics
{
	namespace DX11
	{
		DX11RenderPass::DX11RenderPass(const RenderPassDesc& desc)
		{
			m_PipelineState = reinterpret_cast<DX11PipelineState*>(desc._pipelineState);
			m_PipelineLayout = reinterpret_cast<DX11PipelineLayout*>(desc._pipelineLayout);
			m_RenderTarget = reinterpret_cast<DX11RenderTarget*>(desc._renderTarget);
		}
	}
}