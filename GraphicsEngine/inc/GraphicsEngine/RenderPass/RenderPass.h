#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderPassFlags.h"

#include "GraphicsEngine/Export.h"

#include "GraphicsModule/Core/CommandBufferFlags.h"

namespace Graphics
{
	class PipelineState;
	class PipelineLayout;
	class RenderTarget;
	class Resource;
	class CommandBuffer;

	struct PerFrame;

	class RenderPass
	{
	public:
		RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget);
		RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears);
		virtual ~RenderPass();

		void RegistRenderObject(class RenderObject& renderObject);
		void ClearRenderObject();

		inline const PipelineState* GetPipelineState() const
		{
			return m_PipelineState;
		}

		inline const RenderTarget* GetRenderTarget() const
		{
			return m_RenderTarget;
		}

		void UpdatePerFrame(CommandBuffer* commandBuffer, void* src, uint32 size);
		void UpdatePerDraw(CommandBuffer* commandBuffer, void* src, uint32 size);

		void BeginExcute(CommandBuffer* commandBuffer, PerFrame* perFrameData);
		void Excute(CommandBuffer* commandBuffer);
		void EndExcute(CommandBuffer* commandBuffer);

		void UpdateConstBuffer(CommandBuffer* commandBuffer, RenderObject& renderObject);

		inline void SetPerFrameBuffer(Buffer* perFrame) { m_PerFrameBuffer = perFrame; }
		inline void SetPerDrawBuffer(Buffer* perDraw) { m_PerDrawBuffer = perDraw; }

		inline void SetRenderTarget(RenderTarget* renderTarget) { m_RenderTarget = renderTarget; }

	protected:
		PipelineState* m_PipelineState = nullptr;
		RenderTarget* m_RenderTarget = nullptr;

		std::vector<Resource*> m_Resources;

		std::vector<RenderObject> m_RenderObjects;

		std::vector<AttachmentClear> m_AttachmentClears;

		Buffer* m_PerFrameBuffer;
		Buffer* m_PerDrawBuffer;
		//Buffer* m_PerMaterialBuffer;
	};
}
