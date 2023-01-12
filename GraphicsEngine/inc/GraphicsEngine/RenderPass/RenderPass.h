#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderPassFlags.h"

namespace Graphics
{
	class PipelineState;
	class PipelineLayout;
	class RenderTarget;
	class Resource;
	class CommandBuffer;

	class RenderPass
	{
	public:
		RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget);
		virtual ~RenderPass();

		void RegistRenderObject(class RenderObject& renderObject);

		inline const PipelineState* GetPipelineState() const
		{
			return m_PipelineState;
		}

		inline const RenderTarget* GetRenderTarget() const
		{
			return m_RenderTarget;
		}

		void BeginExcute(CommandBuffer* commandBuffer);
		void Excute(CommandBuffer* commandBuffer);
		void EndExcute(CommandBuffer* commandBuffer);

		void UpdateConstBuffer(CommandBuffer* commandBuffer, RenderObject& renderObject);

	protected:
		PipelineState* m_PipelineState = nullptr;
		RenderTarget* m_RenderTarget = nullptr;

		std::vector<Resource*> m_Resources;

		std::vector<RenderObject> m_RenderObjects;
	};
}
