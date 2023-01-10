#include "GraphicsPCH.h"

#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"

#include "GraphicsModule/Core/DX11/DX11StateManager.h"
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
			CreateFactory();
			QueryVideoAdapters();
			CreateDevice(nullptr);
			CreateStateManager();
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

		void DX11RenderSystem::WriteBuffer(Buffer& buffer, const void* data, uint32 dataSize, uint32 offset)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.UpdateSubresource(m_Context.Get(), data, dataSize, offset);
		}

		void DX11RenderSystem::ReadBuffer(Buffer& buffer, void* data, uint32 dataSize, uint32 offset)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.ReadSubresource(m_Context.Get(), data, dataSize, offset);
		}

		void* DX11RenderSystem::MapBuffer(Buffer& buffer, const CPUAccess access)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			return _castBuffer.Map(m_Context.Get(), access, 0, 0);
		}

		void* DX11RenderSystem::MapBuffer(Buffer& buffer, const CPUAccess access, uint32 offset, uint32 length)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			return _castBuffer.Map(m_Context.Get(), access, offset, length);
		}

		void DX11RenderSystem::UnmapBuffer(Buffer& buffer)
		{
			DX11Buffer& _castBuffer = reinterpret_cast<DX11Buffer&>(buffer);

			_castBuffer.Unmap(m_Context.Get());
		}

		void DX11RenderSystem::WriteTexture(Texture& texture)
		{
			assert(false);
		}

		void DX11RenderSystem::ReadTexture(Texture& texture)
		{
			assert(false);
		}

		CommandBuffer* DX11RenderSystem::CreateCommandBuffer(uuid uuid, const CommandBufferDesc& desc)
		{
			return TakeOwnership(m_CommandBufferContainer, uuid, MakeUnique<DX11CommandBuffer>(m_Device.Get(), m_Context, m_StateManager, desc));;
		}

		void DX11RenderSystem::Release(CommandBuffer& commandBuffer)
		{
			RemoveFromUniqueUnorderedMap(m_CommandBufferContainer, &commandBuffer);
		}

		Buffer* DX11RenderSystem::CreateBuffer(uuid uuid, const BufferDesc& desc, const void* initData /*= nullptr*/)
		{
			return TakeOwnership(m_BufferContainer, uuid, MakeUnique<DX11Buffer>(m_Device.Get(), desc, initData));
		}

		void DX11RenderSystem::Release(Buffer& buffer)
		{
			RemoveFromUniqueUnorderedMap(m_BufferContainer, &buffer);
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
			RemoveFromUniqueUnorderedMap(m_TextureContainer, &texture);
		}

		Graphics::Sampler* DX11RenderSystem::CreateSampler(uuid uuid, const SamplerDesc& desc)
		{
			return TakeOwnership(m_SamplerContainer, uuid, MakeUnique<DX11Sampler>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(Sampler& sampler)
		{
			RemoveFromUniqueUnorderedMap(m_SamplerContainer, &sampler);
		}

		Graphics::RenderPass* DX11RenderSystem::CreateRenderPass(uuid uuid, const RenderPassDesc& desc)
		{
			DX11RenderPass* _pass = TakeOwnership(m_RenderPassContainer, uuid, MakeUnique<DX11RenderPass>());

			return _pass;
		}

		void DX11RenderSystem::Release(RenderPass& renderPass)
		{
			RemoveFromUniqueUnorderedMap(m_RenderPassContainer, &renderPass);
		}

		Graphics::RenderTarget* DX11RenderSystem::CreateRenderTarget(uuid uuid, const RenderTargetDesc& desc)
		{
			return TakeOwnership(m_RenderTargetContainer, uuid, MakeUnique<DX11RenderTarget>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(RenderTarget& renderTarget)
		{
			RemoveFromUniqueUnorderedMap(m_RenderTargetContainer, &renderTarget);
		}

		Graphics::Shader* DX11RenderSystem::CreateShader(uuid uuid, const ShaderDesc& desc)
		{
			return TakeOwnership(m_ShaderContainer, uuid, MakeUnique<DX11Shader>(m_Device.Get(), desc));
		}

		void DX11RenderSystem::Release(Shader& shader)
		{
			RemoveFromUniqueUnorderedMap(m_ShaderContainer, &shader);
		}

		Graphics::PipelineLayout* DX11RenderSystem::CreatePipelineLayout(uuid uuid, const PipelineLayoutDesc& desc)
		{
			return TakeOwnership(m_PipelineLayoutContainer, uuid, MakeUnique<DX11PipelineLayout>(desc));
		}

		void DX11RenderSystem::Release(PipelineLayout& pipelineLayout)
		{
			RemoveFromUniqueUnorderedMap(m_PipelineLayoutContainer, &pipelineLayout);
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
			HR(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory)), "faild to create IDXGIFactory");
		}

		void DX11RenderSystem::QueryVideoAdapters()
		{
			ComPtr<IDXGIAdapter> _adapter;
			uint32 _index = 0;
			
			while (m_Factory->EnumAdapters(_index, _adapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
			{
				ComPtr<IDXGIOutput> _output;

				uint32 _outIndex = 0;

				DXGI_ADAPTER_DESC desc;
				_adapter->GetDesc(&desc);

				VideoAdapterDesc _adapterDesc;

				_adapterDesc._name = tstring(desc.Description);
				_adapterDesc._vendorID = desc.VendorId;
				_adapterDesc._videoMemory = static_cast<uint64_t>(desc.DedicatedVideoMemory);

				while (_adapter->EnumOutputs(_outIndex, _output.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND)
				{
					UINT _numModes = 0;
					std::vector<DXGI_MODE_DESC> _displayModes;
					DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UNORM;

					HRESULT hr = _output->GetDisplayModeList(_format, DXGI_ENUM_MODES_INTERLACED, &_numModes, NULL);

					_displayModes.resize(_numModes);

					hr = _output->GetDisplayModeList(_format, 0, &_numModes, _displayModes.data());

					VideoOutputDesc _videoOutputDesc;

					for (size_t i = 0; i < _numModes; i++)
					{
						DisplayModeDesc _modeDesc;

						_modeDesc._resolution._width	= _displayModes[i].Width;
						_modeDesc._resolution._height	= _displayModes[i].Height;
						_modeDesc._refreshRate = (_displayModes[i].RefreshRate.Denominator > 0 ? _displayModes[i].RefreshRate.Numerator / _displayModes[i].RefreshRate.Denominator : 0);

						_videoOutputDesc._displayModes.push_back(_modeDesc);
					}

					m_VideoAdapters.push_back(_adapterDesc);
					_outIndex++;
				}

				_index++;
			}
		}

		void DX11RenderSystem::CreateDevice(IDXGIAdapter* adapter)
		{
			std::vector<D3D_FEATURE_LEVEL> featureLevels =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};


			HRESULT hr = 0;
#if defined(_DEBUG) || defined(DEBUG)
			if (!CreateDeviceWithFlags(adapter, featureLevels, D3D11_CREATE_DEVICE_DEBUG, hr))
				CreateDeviceWithFlags(adapter, featureLevels, 0, hr);
#else
			CreateDeviceWithFlags(adapter, featureLevels, 0, hr);
#endif
			uint32 _4xMsaaQuality = 0;

			m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);
			assert(_4xMsaaQuality > 0);


		}

		bool DX11RenderSystem::CreateDeviceWithFlags(IDXGIAdapter* adapter, const std::vector<D3D_FEATURE_LEVEL>& featureLevels, UINT flags, HRESULT& hr)
		{
			for (D3D_DRIVER_TYPE driver : { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE })
			{
				hr = D3D11CreateDevice(
					adapter,                                    // Video adapter
					driver,                                     // Driver type
					0,                                          // Software rasterizer module (none)
					flags,                                      // Flags
					featureLevels.data(),                       // Feature level
					static_cast<UINT>(featureLevels.size()),    // Num feature levels
					D3D11_SDK_VERSION,                          // SDK version
					m_Device.ReleaseAndGetAddressOf(),           // Output device
					&m_FetureLevel,                             // Output feature level
					m_Context.ReleaseAndGetAddressOf()           // Output device context
				);
				if (SUCCEEDED(hr))
					return true;
			}
			return false;
		}

		void DX11RenderSystem::CreateStateManager()
		{
			m_StateManager = std::make_shared<DX11StateManager>(m_Device.Get(), m_Context);
		}

		void DX11RenderSystem::Release(PipelineState& pipelineState)
		{
			RemoveFromUniqueUnorderedMap(m_PipelineStateContainer, &pipelineState);
		}
	}
}