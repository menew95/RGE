#pragma once

#include "Common.h"

namespace Graphics
{
	namespace Metal
	{
		class DeviceContext;
		class DepthStencilTexture;
		class RenderTargetTexture;
	}

	class RenderPipeLine
	{
	public:
		RenderPipeLine(std::shared_ptr<Metal::DeviceContext>&);
		virtual ~RenderPipeLine();

		virtual void BeginRender();
		virtual void Render();
		virtual void EndRender();

	protected:
		std::shared_ptr<Metal::DepthStencilTexture> m_DepthStencilTexture;
		std::shared_ptr<Metal::RenderTargetTexture> m_RenderTargetTexture;
	};
}