#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"
#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"
#include "GraphicsModule/Core/DX11/DX11CommandBuffer.h"
#include "GraphicsModule/Core/DX11/DX11Buffer.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/DX11Sampler.h"
#include "GraphicsModule/Core/DX11/DX11RenderPass.h"
#include "GraphicsModule/Core/DX11/DX11RenderTarget.h"
#include "GraphicsModule/Core/DX11/DX11Shader.h"
#include "GraphicsModule/Core/DX11/DX11PipelineLayout.h"
#include "GraphicsModule/Core/DX11/DX11PipelineState.h"


namespace Graphics
{
	namespace DX11
	{

		DX11RenderSystem::DX11RenderSystem()
		{

		}

		DX11RenderSystem::~DX11RenderSystem()
		{

		}

		SwapChain* DX11RenderSystem::CreateSwapChain(const SwapChainDesc& desc)
		{
			DX11SwapChain* _swapChain = nullptr;


			return _swapChain;
		}

		void DX11RenderSystem::Release(SwapChain& swapChain)
		{

		}

		CommandBuffer* DX11RenderSystem::CreateCommandBuffer(const CommandBufferDesc& desc)
		{
			DX11CommandBuffer* _commandBuffer = nullptr;

			return _commandBuffer;
		}

		void DX11RenderSystem::Release(CommandBuffer& commandBuffer)
		{

		}

		Buffer* DX11RenderSystem::CreateBuffer(const BufferDesc& desc, const void* initData /*= nullptr*/)
		{
			DX11Buffer* _buffer = new DX11Buffer(m_Device.Get(), desc, initData);

			return _buffer;
		}

		void DX11RenderSystem::Release(Buffer& buffer)
		{

		}

		Texture* DX11RenderSystem::CreateTexture(const TextureDesc& desc, const class SrcImageDesc* imageDesc /*= nullptr*/)
		{
			DX11Texture* _texture = nullptr;

			if (imageDesc != nullptr)
			{

			}

			switch (desc._textureType)
			{
				case TextureType::Texture1D:
				case TextureType::Texture1DArray:
				{
					CreateTexture()
					break;
				}
				case TextureType::Texture2D:
				case TextureType::Texture2DArray:
				case TextureType::TextureCube:
				case TextureType::TextureCubeArray:
				case TextureType::Texture3D:
				{

					break;
				}
				case TextureType::Texture2DMS:
				case TextureType::Texture2DMSArray:
				{
					CreateTexture
					break;
				}
				default:
					AssertMessageBox(false, "failed to create texture with invaild texture type");
					break;
			}

			return _texture;
		}

		void DX11RenderSystem::Release(Texture& texture)
		{

		}

		Graphics::Sampler* DX11RenderSystem::CreateSampler(const SamplerDesc& desc)
		{
			return new DX11Sampler(m_Device.Get(), desc);
		}

		void DX11RenderSystem::Release(Sampler& sampler)
		{

		}

		Graphics::RenderPass* DX11RenderSystem::CreateRenderPass(const RenderPassDesc& desc)
		{
			DX11RenderPass* _pass = nullptr;

			return _pass;
		}

		void DX11RenderSystem::Release(RenderPass& renderPass)
		{

		}

		Graphics::RenderTarget* DX11RenderSystem::CreateRenderTarget(const RenderTargetDesc& desc)
		{
			DX11RenderTarget* _rt = nullptr;

			return _rt;
		}

		void DX11RenderSystem::Release(RenderTarget& renderTarget)
		{

		}

		Graphics::Shader* DX11RenderSystem::CreateShader(const ShaderDesc& desc)
		{
			DX11Shader* _shader = nullptr;

			return _shader;
		}

		void DX11RenderSystem::Release(Shader& shader)
		{

		}

		Graphics::PipelineLayout* DX11RenderSystem::CreatePipelineLayout(const PipelineLayoutDesc& desc)
		{
			DX11PipelineLayout* _layout = nullptr;

			return _layout;
		}

		void DX11RenderSystem::Release(PipelineLayout& pipelineLayout)
		{

		}

		Graphics::PipelineState* DX11RenderSystem::CreatePipelineState(const GraphicsPipelineDesc& desc)
		{
			DX11PipelineState* _pipeline = nullptr;

			return _pipeline;
		}

		DXGI_SAMPLE_DESC DX11RenderSystem::FindSuitableSampleDesc(ID3D11Device* device, DXGI_FORMAT format, uint32 maxSampleCount)
		{
			for (; maxSampleCount > 1; --maxSampleCount)
			{
				UINT numQualityLevels = 0;
				if (device->CheckMultisampleQualityLevels(format, maxSampleCount, &numQualityLevels) == S_OK)
				{
					if (numQualityLevels > 0)
						return { maxSampleCount, numQualityLevels - 1 };
				}
			}
			return { 1u, 0u };
		}

		void DX11RenderSystem::CreateFactory()
		{

		}

		void DX11RenderSystem::Release(PipelineState& pipelineState)
		{
			/* Create DXGI factory */
			HR(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory)));
		}
	}
}