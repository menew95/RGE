#pragma once

#include "GraphicsModule/Core/RenderPass.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11PipelineState;
		class DX11PipelineLayout;
		class DX11RenderTarget;

		class DX11RenderPass : public RenderPass
		{
		public:
			DX11RenderPass(const RenderPassDesc& desc);



			inline const DX11PipelineState* GetPipelineState() const
			{
				return m_PipelineState;
			}

			inline const DX11PipelineLayout* GetPipelineLayout() const
			{
				return m_PipelineLayout;
			}

			inline const DX11RenderTarget* GetRenderTarget() const
			{
				return m_RenderTarget;
			}

		private:
			DX11PipelineState* m_PipelineState = nullptr;
			DX11PipelineLayout* m_PipelineLayout = nullptr;
			DX11RenderTarget* m_RenderTarget = nullptr;
		};
	}
}