#pragma once

#include "Common.h"

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/CommandBufferFlags.h"
#include "GraphicsModule/Core/ShaderFlags.h"
#include "GraphicsModule/Core/ResourceFlags.h"

namespace Graphics
{
	class Buffer;
	class Texture;
	class Sampler;
	class PipelineState;
	class PipelineLayout;
	class Resource;
	class RenderPass;
	class RenderTarget;

	class GRAPHICS_DLL_DECLSPEC CommandBuffer
	{
	public:
		CommandBuffer() = default;
		virtual ~CommandBuffer() = default;

		virtual void Begin() abstract;
		virtual void End() abstract;
		virtual void Execute(CommandBuffer& deferredCommandBuffer) abstract;

		/* ----- Buffer ----- */
		virtual void UpdateBuffer(Buffer& destBuffer, uint32 destBufferOffset, const void* data, uint32 dataSize) abstract;

		virtual void CopyBuffer(Buffer& dstBuffer, uint32 dstOffset, Buffer& srcBuffer, uint32 srcOffset, uint32 size) abstract;

		/* ----- Viewport and Scissor ----- */

		virtual void SetViewport(const Math::Viewport& viewport) abstract;
		virtual void SetViewports(uint32 numViewports, const Math::Viewport* viewport) abstract;

		virtual void SetScissor(const Math::Scissor& scissor) abstract;
		virtual void SetScissors(uint32 numScissors, const Math::Scissor* scissors) abstract;

		/* ----- Input Assembly ------ */

		virtual void SetVertexBuffer(Buffer& buffer) abstract;

		virtual void SetIndexBuffer(Buffer& buffer) abstract;

		/* ----- Resources ----- */

		virtual void SetResource(Resource& resource, uint32 slot, long bindFlags, long stageFlags = StageFlags::AllStages) abstract;
		
		virtual void SetResources(PipelineLayout& pipelineLayout) abstract;
		
		virtual void ResetResourceSlots(const ResourceType resourceType, uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages) abstract;
	
		/* ----- Render Passes ----- */

		virtual void BeginRenderPass(
			RenderPass& renderPass,
			std::uint32_t       numClearValues,
			const ClearValue* clearValues
		) abstract;

		virtual void EndRenderPass() abstract;

		virtual void Clear(long flags, const ClearValue& clearValue = {}) abstract;
		virtual void ClearAttachments(uint32 numAttachments, const AttachmentClear* attachments) abstract;

		/* ----- Render Target ----- */
		virtual void SetRenderTarget(RenderTarget& renderTarget, uint32 numAttachments, const AttachmentClear* attachments) abstract;

		/* ----- Pipeline States ----- */

		virtual void SetPipelineState(PipelineState& pipelineState) abstract;

		/* ----- Drawing ----- */

		virtual void Draw(uint32 numVertices, uint32 firstVertex) abstract;
		virtual void DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 offset = 0) abstract;
		virtual void DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance = 0) abstract;
		virtual void DrawIndexedInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 vertexOffset, uint32 firstInstance = 0) abstract;
	
		/* ----- Compute ----- */

		virtual void Dispatch(uint32 numWorkGroupsX, uint32 numWorkGroupsY, uint32 numWorkGroupsZ) abstract;
	};

}