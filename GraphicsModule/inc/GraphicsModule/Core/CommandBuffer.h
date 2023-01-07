#pragma once

#include "Common.h"
#include "GraphicsModule/Core/CommandBufferFlags.h"

namespace Graphics
{
	class Buffer;
	class PipelineState;

	class CommandBuffer
	{
	public:
		CommandBuffer() = default;
		virtual ~CommandBuffer() = default;

		virtual void Begin() abstract;
		virtual void End() abstract;
		virtual void Execute(CommandBuffer& deferredCommandBuffer) abstract;

		virtual void UpdateBuffer(
			Buffer& dstBuffer,
			uint32   dstOffset,
			const void* data,
			uint32   dataSize
		) abstract;

		virtual void Draw(uint32 numVertices, uint32 firstVertex) abstract;
		virtual void DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 offset = 0) abstract;
		virtual void DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance = 0) abstract;
		virtual void DrawIndexedInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 vertexOffset, uint32 firstInstance = 0) abstract;
		
		virtual void SetBuffer() abstract;

		virtual void SetViewport(const Math::Viewport& viewport) abstract;
		virtual void SetViewports(uint32 numViewports, const Math::Viewport* viewport) abstract;

		virtual void SetScissor(const Math::Scissor& scissor) abstract;
		virtual void SetScissors(uint32 numScissors, const Math::Scissor* scissors) abstract;

		virtual void SetPipelineState(PipelineState& pipelineState) abstract;

		virtual void SetVertexBuffer(Buffer& buffer) abstract;

		virtual void SetIndexBuffer(Buffer& buffer) abstract;
	};

}