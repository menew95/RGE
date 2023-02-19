#pragma once

#include "GraphicsModule\Core\RenderSystem.h"
#include "GraphicsModule\Core\DX11\Direct3D11.h"
#include "GraphicsModule\Core\Common\ComPtr.h"


namespace Graphics
{
	namespace DX11
	{
		/// @brief 그래픽스 오브젝트들을 생성 및 관리 하는 클래스
		class DX11RenderSystem : public RenderSystem
		{
		public: 
			DX11RenderSystem();
			~DX11RenderSystem() override;

			///* ----- DeviceContext ---- */
			//DeviceContext* CreateDeviceContext(uuid uuid, const DeviceContextDesc& desc) override;
			//void Release(DeviceContext& context) override

			/* ----- SwapChain ----- */
			SwapChain* CreateSwapChain(uuid uuid, const SwapChainDesc& desc) override;
			void Release(SwapChain& swapChain) override;

			/* ----- Command Buffer ----- */
			CommandBuffer* CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc) override;
			void Release(CommandBuffer& commandBuffer) override;

			/* ----- Buffer ----- */
			Buffer* CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData = nullptr) override;
			void Release(Buffer& buffer) override;

			void WriteBuffer(Buffer& buffer, const void* data, uint32 dataSize, uint32 offset) override;
			void ReadBuffer(Buffer& buffer, void* data, uint32 dataSize, uint32 offset) override;

			void* MapBuffer(Buffer& buffer, const CPUAccess access) override;
			void* MapBuffer(Buffer& buffer, const CPUAccess access, uint32 offset, uint32 length) override;
			void UnmapBuffer(Buffer& buffer) override;

			/* ----- Textures ----- */
			Texture* CreateTexture(uuid uuid, const TextureDesc& desc, const struct ImageDesc* imageDesc = nullptr) override;
			void Release(Texture& texture) override;

			void WriteTexture(Texture& texture, const TextureRegion& textureRegion, const ImageDesc& imageDesc) override;
			void ReadTexture(Texture& texture) override;

			/* ----- Samplers ---- */
			Sampler* CreateSampler(uuid uuid, const SamplerDesc& desc) override;
			void Release(Sampler& sampler) override;

			///* ----- Render Passes ----- */
			//RenderPass* CreateRenderPass(uuid uuid, const RenderPassDesc& desc) override;
			//void Release(RenderPass& renderPass) override;

			/* ----- Render Targets ----- */
			RenderTarget* CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc) override;
			void Release(RenderTarget& renderTarget) override;

			/* ----- Shader ----- */
			Shader* CreateShader(uuid uuid, const ShaderDesc& desc) override;
			void Release(Shader& shader) override;

			/* ----- Pipeline Layouts ----- */
			PipelineLayout* CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc) override;
			void Release(PipelineLayout& pipelineLayout) override;

			/* ----- Pipeline States ----- */
			PipelineState* CreatePipelineState(uuid uuid, const GraphicsPipelineDesc& desc) override;
			void Release(PipelineState& pipelineState) override;

			static DXGI_SAMPLE_DESC FindSuitableSampleDesc(ID3D11Device* device, DXGI_FORMAT format, uint32 maxSampleCount);
		
		private:
			void CreateFactory();
			void QueryVideoAdapters();
			void CreateDevice(IDXGIAdapter* adapter);
			bool CreateDeviceWithFlags(IDXGIAdapter* adapter, const std::vector<D3D_FEATURE_LEVEL>& featureLevels, UINT flags, HRESULT& hr);
			void CreateStateManager();

			ComPtr<IDXGIFactory> m_Factory;
			ComPtr<IDXGIAdapter> m_Adapter;

			ComPtr<ID3D11Device> m_Device;
			ComPtr<ID3D11DeviceContext> m_Context;

			std::shared_ptr<class DX11StateManager> m_StateManager;

			D3D_FEATURE_LEVEL m_FetureLevel = D3D_FEATURE_LEVEL_11_0;

			std::vector<VideoAdapterDesc> m_VideoAdapters;

			template<typename T>
			using Container = std::unordered_map<uuid, std::unique_ptr<T>>;

			Container <class DX11SwapChain>			m_SwapChainContainer;
			Container <class DX11CommandBuffer>		m_CommandBufferContainer;
			Container <class DX11Buffer>			m_BufferContainer;
			Container <class DX11Texture>			m_TextureContainer;
			Container <class DX11Sampler>			m_SamplerContainer;
			//Container <class DX11RenderPass>		m_RenderPassContainer;
			Container <class DX11RenderTarget>		m_RenderTargetContainer;
			Container <class DX11Shader>			m_ShaderContainer;
			Container <class DX11PipelineLayout>	m_PipelineLayoutContainer;
			Container <class DX11PipelineState>		m_PipelineStateContainer;
		};
	}
}