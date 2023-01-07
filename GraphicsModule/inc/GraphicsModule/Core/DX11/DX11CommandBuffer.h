#pragma once

#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11CommandList;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Graphics
{
	class Buffer;
	class Texture;
	class Sampler;

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
			void End() override;

			void Execute(CommandBuffer& deferredCommandBuffer) override;

			void UpdateBuffer(
				Buffer& dstBuffer,
				uint32   dstOffset,
				const void* data,
				uint32   dataSize
			) override;


			void Draw(uint32 numVertices, uint32 firstVertex) override;
			void DrawIndexed(uint32 numIndices, uint32 firstIndex, uint32 offset = 0) override;
			void DrawInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 firstInstance) override;
			void DrawIndexedInstanced(uint32 numVertices, uint32 firstVertex, uint32 numInstances, uint32 vertexOffset, uint32 firstInstance) override;

			void SetViewport(const Math::Viewport& viewport) override;
			void SetViewports(uint32 numViewports, const Math::Viewport* viewport) override;

			void SetScissor(const Math::Scissor& scissor) override;
			void SetScissors(uint32 numScissors, const Math::Scissor* scissors) override;

			void SetPipelineState(PipelineState&) override;

			void SetVertexBuffer(Buffer& buffer) override;

			void SetIndexBuffer(Buffer& buffer) override;

		private:
			void SetBuffer(Buffer* buffer, uint32 slot, uint32 bindFlags, uint32 stageFlags);
			void SetTexture(Texture* texture, uint32 slot, uint32 bindFlags, uint32 stageFlags);
			void SetSampler(Sampler* sampler, uint32 slot, uint32 stageFlags);

			ID3D11Device* m_Device = nullptr;
			ComPtr<ID3D11DeviceContext>         m_Context;
			ComPtr<ID3D11CommandList>           m_CommandList;

			bool m_IsDeferredContext = false;
			bool m_IsSecondaryCmdBuffer = false;
			std::shared_ptr<DX11StateManager>  m_StateManager;

			std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
			ID3D11DepthStencilView* m_DepthStencilView;

			class DX11RenderTarget* boundRenderTarget = nullptr;
		};
	}
}