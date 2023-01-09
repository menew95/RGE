#include "GraphicsPCH.h"

#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"

#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
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

		SwapChain* DX11RenderSystem::CreateSwapChain(uuid uuid, const SwapChainDesc& desc)
		{
			return TakeOwnership(m_SwapChainContainer, uuid, MakeUnique<DX11SwapChain>(m_Factory.Get(), m_Device.Get(), desc));
		}



		void DX11RenderSystem::Release(SwapChain& swapChain)
		{
			RemoveFromUniqueUnorderedMap(m_SwapChainContainer, &swapChain);
		}

		CommandBuffer* DX11RenderSystem::CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc)
		{
			DX11CommandBuffer* _commandBuffer = nullptr;

			return _commandBuffer;
		}

		void DX11RenderSystem::Release(CommandBuffer& commandBuffer)
		{
			//RemoveFromUniqueUnorderedMap(m_CommandBufferContainer, &commandBuffer);
		}

		Buffer* DX11RenderSystem::CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData /*= nullptr*/)
		{
			return TakeOwnership(m_BufferContainer, uuid, MakeUnique<DX11Buffer>(m_Device.Get(), desc, initData));
		}

		void DX11RenderSystem::Release(Buffer& buffer)
		{
			//RemoveFromUniqueUnorderedMap(m_BufferContainer, &buffer);
		}

		Texture* DX11RenderSystem::CreateTexture(uuid uuid, const TextureDesc& desc, const ImageDesc* imageDesc /*= nullptr*/)
		{
			DX11Texture* _texture = TakeOwnership(m_TextureContainer, uuid, MakeUnique<DX11Texture>(desc));

			if (imageDesc == nullptr)
			{
				switch (desc._textureType)
				{
					case TextureType::Texture1D:
					case TextureType::Texture1DArray:
					{
						_texture->CreateTexture1D(m_Device.Get(), desc);
						break;
					}
					case TextureType::Texture2D:
					case TextureType::Texture2DArray:
					case TextureType::TextureCube:
					case TextureType::TextureCubeArray:
					case TextureType::Texture2DMS:
					case TextureType::Texture2DMSArray:
					{
						_texture->CreateTexture2D(m_Device.Get(), desc);
						break;
					}
					case TextureType::Texture3D:
					{
						_texture->CreateTexture3D(m_Device.Get(), desc);
						break;
					}
					default:
						AssertMessageBox(false, "failed to create texture with invaild texture type");
						break;
				}
				if ((desc._bindFlags & BindFlags::ShaderResource) != 0)
				{
					_texture->CreateShaderResourceView(m_Device.Get(), 0, desc._mipLevels, 0, desc._arrayLayers);
				}
			}
			else
			{
				_texture->CreateTextureFromFile(m_Device.Get(), *imageDesc);
			}

			return _texture;
		}

		void DX11RenderSystem::Release(Texture& texture)
		{
			//RemoveFromUniqueUnorderedMap(m_TextureContainer, &texture);
		}

		Graphics::Sampler* DX11RenderSystem::CreateSampler(uuid uuid, const SamplerDesc& desc)
		{
			return TakeOwnership(m_SamplerContainer, uuid, MakeUnique<DX11Sampler>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(Sampler& sampler)
		{
			//RemoveFromUniqueUnorderedMap(m_SamplerContainer, &sampler);
		}

		Graphics::RenderPass* DX11RenderSystem::CreateRenderPass(uuid uuid, const RenderPassDesc& desc)
		{
			DX11RenderPass* _pass = TakeOwnership(m_RenderPassContainer, uuid, MakeUnique<DX11RenderPass>());

			return _pass;
		}

		void DX11RenderSystem::Release(RenderPass& renderPass)
		{
			//RemoveFromUniqueUnorderedMap(m_RenderPassContainer, &renderPass);
		}

		Graphics::RenderTarget* DX11RenderSystem::CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc)
		{
			DX11RenderTarget* _rt = TakeOwnership(m_RenderTargetContainer, uuid, MakeUnique<DX11RenderTarget>(m_Device.Get(), desc));

			return _rt;
		}

		void DX11RenderSystem::Release(RenderTarget& renderTarget)
		{
			//RemoveFromUniqueUnorderedMap(m_RenderTargetContainer, &renderTarget);
		}

		Graphics::Shader* DX11RenderSystem::CreateShader(uuid uuid, const ShaderDesc& desc)
		{
			DX11Shader* _shader = TakeOwnership(m_ShaderContainer, uuid, MakeUnique<DX11Shader>());

			return _shader;
		}

		void DX11RenderSystem::Release(Shader& shader)
		{
			//RemoveFromUniqueUnorderedMap(m_ShaderContainer, &shader);
		}

		Graphics::PipelineLayout* DX11RenderSystem::CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc)
		{
			DX11PipelineLayout* _layout = TakeOwnership(m_PipelineLayoutContainer, uuid, MakeUnique<DX11PipelineLayout>(desc));

			return _layout;
		}

		void DX11RenderSystem::Release(PipelineLayout& pipelineLayout)
		{
			//RemoveFromUniqueUnorderedMap(m_PipelineLayoutContainer, &pipelineLayout);
		}

		Graphics::PipelineState* DX11RenderSystem::CreatePipelineState(uuid uuid, const GraphicsPipelineDesc& desc)
		{
			DX11PipelineState* _pipeline = TakeOwnership(m_PipelineStateContainer, uuid, MakeUnique<DX11PipelineState>(m_Device.Get(), desc));

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
			//RemoveFromUniqueUnorderedMap(m_PipelineStateContainer, &pipelineState);
		}
	}
}