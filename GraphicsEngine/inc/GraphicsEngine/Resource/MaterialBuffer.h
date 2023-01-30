#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"


namespace Graphics
{
	class GRAPHICSENGINE_DLL_DECLSPEC MaterialBuffer : public ResourceBuffer
	{
	public:
		MaterialBuffer(Graphics::RenderSystem* renderSystem, Graphics::PipelineLayout* pipelineLayout);
		~MaterialBuffer() override;

		inline Graphics::PipelineLayout* GetPipelineLayout() const
		{
			return m_PipelineLayout;
		}

		inline void SetPipelineLayout(Graphics::PipelineLayout* layout)
		{
			m_PipelineLayout = layout;
		}

		virtual void SetResource(uint32 i, Graphics::Resource* resource);

		inline void SetRenderPass(Graphics::RenderPass* renderPass)
		{
			m_RenderPass = renderPass;
		}

		inline Graphics::RenderPass* GetRenderPass()
		{
			return m_RenderPass;
		}

		void RegistRenderObject(class RenderObject& renderObject);

	private:
		Graphics::PipelineLayout* m_PipelineLayout;
		Graphics::RenderPass* m_RenderPass;
	};
}