#pragma once

#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

#include <d3d11_1.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11CommandList;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Graphics
{
	class SwapChain;

	namespace DX11
	{
		class DX11StateManager;

		class DX11CommandBuffer final : public CommandBuffer
		{
		public:
			DX11CommandBuffer(ID3D11Device* device,
				const ComPtr<ID3D11DeviceContext>& context,
				const std::shared_ptr<class DX11StateManager>& stateManager,
				const CommandBufferDesc& desc);
			~DX11CommandBuffer();


			void Begin() override;

			void BeginEvent(const wchar_t* event) override;

			void End() override;

			void EndEvent() override;

			void Execute(CommandBuffer& deferredCommandBuffer) override;

			/* ----- Buffer ----- */
			void UpdateBuffer(Buffer& destBuffer, uint32 destBufferOffset, const void* data, uint32 dataSize) override;

			void CopyBuffer(Buffer& dstBuffer, uint32 dstOffset, Buffer& srcBuffer, uint32 srcOffset, uint32 size) override;

			void GenerateMips(Texture& texture) override;
			void GenerateMips(Texture& texture, const TextureSubresource& subresource) override;

			/* ----- Viewport and Scissor ----- */

			void SetViewport(const Math::Viewport& viewport) override;
			void SetViewports(uint32 numViewports, const Math::Viewport* viewport) override;

			void SetScissor(const Math::Scissor& scissor) override;
			void SetScissors(uint32 numScissors, const Math::Scissor* scissors) override;

			/* ----- Input Assembly ------ */

			void SetVertexBuffer(Buffer& buffer) override;

			void SetIndexBuffer(Buffer& buffer) override;

			/* ----- Resources ----- */

			void SetResource(Resource& resource, uint32 slot, long bindFlags, long stageFlags = StageFlags::AllStages) override;
			
			void SetResources(PipelineLayout& pipelineLayout) override;

			void ResetResourceSlots(const ResourceType  resourceType, uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages) override;

			void ClearState() override;

			/* ----- Render Passes ----- */

			void BeginRenderPass(
				RenderPass& renderPass,
				uint32       numClearValues,
				const ClearValue* clearValues
			) override;

			void EndRenderPass() override;

			void Clear(long flags, const ClearValue& clearValue = {}) override;
			void ClearAttachments(uint32 numAttachments, const AttachmentClear* attachments) override;

			/* ----- Render Target ----- */

			void SetRenderTarget(RenderTarget& renderTarget, uint32 numAttachments, const AttachmentClear* attachments) override;

			void BindSwapChain(SwapChain* swapChain);

			void BindFramebufferView(uint32 numRTVs, ID3D11RenderTargetView* const* renderTargetViews, ID3D11DepthStencilView* depthStencilView);

			/* ----- Pipeline States ----- */

			void SetPipelineState(PipelineState& pipelineState) override;

			/* ----- Drawing ----- */

			void Draw(uint32 numVertices, uint32 firstVertex) override;
			void DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 offset = 0) override;
			void DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance = 0) override;
			void DrawIndexedInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 vertexOffset, uint32 firstInstance = 0) override;

			/* ----- Compute ----- */

			void Dispatch(uint32 numWorkGroupsX, uint32 numWorkGroupsY, uint32 numWorkGroupsZ) override;
			void DispatchIndirect(Buffer& buffer, uint32 offset) override;

		private:
			void SetBuffer(Buffer& buffer, uint32 slot, uint32 bindFlags, uint32 stageFlags);
			void SetTexture(Texture& texture, uint32 slot, uint32 bindFlags, uint32 stageFlags);
			void SetSampler(Sampler& sampler, uint32 slot, uint32 stageFlags);

			void ResetBufferSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages);
			void ResetTextureSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages);
			void ResetSamplerSlots(uint32 firstSlot, uint32 numSlots, long bindFlags, long stageFlags = StageFlags::AllStages);

			void ResetResourceSlotsSRV(uint32 firstSlot, uint32 numSlots, long stageFlags);
			void ResetResourceSlotsUAV(uint32 firstSlot, uint32 numSlots, long stageFlags);

			ID3D11Device* m_Device = nullptr;
			ComPtr<ID3D11DeviceContext>         m_Context;
			ComPtr<ID3D11CommandList>           m_CommandList;

			ComPtr<ID3DUserDefinedAnnotation>	m_User;

			bool m_IsDeferredContext = false;
			bool m_IsSecondaryCmdBuffer = false;
			std::shared_ptr<DX11StateManager>  m_StateManager;

			struct D3D11FramebufferView
			{
				UINT                            _numRenderTargetViews = 0;
				ID3D11RenderTargetView* const*	_renderTargetViews = nullptr;
				ID3D11DepthStencilView*			_depthStencilView = nullptr;
			};

			D3D11FramebufferView                m_FramebufferView;

			class DX11RenderTarget* m_BoundRenderTarget = nullptr;
		};
	}
}

