#pragma once

#include "GraphicsModule\Core\RenderSystem.h"
#include "GraphicsModule\Core\DX11\Direct3D11.h"
#include "GraphicsModule\Core\Common\ComPtr.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11RenderSystem : public RenderSystem
		{
		public: 
			DX11RenderSystem();
			~DX11RenderSystem() override;

			/* ----- SwapChain ----- */
			SwapChain* CreateSwapChain(const SwapChainDesc& desc) override;
			void Release(SwapChain& swapChain) override;

			/* ----- Command Buffer ----- */
			CommandBuffer* CreateCommandBuffer(const CommandBufferDesc& desc) override;
			void Release(CommandBuffer& commandBuffer) override;

			/* ----- Buffer ----- */
			Buffer* CreateBuffer(const BufferDesc& desc, const void* initData = nullptr) override;
			void Release(Buffer& buffer) override;

			/* ----- Textures ----- */
			Texture* CreateTexture(const TextureDesc& desc, const class SrcImageDesc* imageDesc = nullptr) override;
			void Release(Texture& texture) override;

			/* ----- Samplers ---- */
			Sampler* CreateSampler(const SamplerDesc& desc) override;
			void Release(Sampler& sampler) override;

			/* ----- Render Passes ----- */
			RenderPass* CreateRenderPass(const RenderPassDesc& desc) override;
			void Release(RenderPass& renderPass) override;

			/* ----- Render Targets ----- */
			RenderTarget* CreateRenderTarget(const RenderTargetDesc& desc) override;
			void Release(RenderTarget& renderTarget) override;

			/* ----- Shader ----- */
			Shader* CreateShader(const ShaderDesc& desc) override;
			void Release(Shader& shader) override;

			/* ----- Pipeline Layouts ----- */
			PipelineLayout* CreatePipelineLayout(const PipelineLayoutDesc& desc) override;
			void Release(PipelineLayout& pipelineLayout) override;

			/* ----- Pipeline States ----- */
			PipelineState* CreatePipelineState(const GraphicsPipelineDesc& desc) override;
			void Release(PipelineState& pipelineState) override;

			static DXGI_SAMPLE_DESC FindSuitableSampleDesc(ID3D11Device* device, DXGI_FORMAT format, uint32 maxSampleCount);
		
		private:
			void CreateFactory();


			ComPtr<IDXGIFactory> m_Factory;
			ComPtr<IDXGIAdapter> m_Adapter;

			ComPtr<ID3D11Device> m_Device;
			ComPtr<ID3D11DeviceContext> m_DeviceContext;
		};
	}
}