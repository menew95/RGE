#include "GraphicsPCH.h"
#include "GraphicsModule/Window.h"
#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"
#include "GraphicsModule/Core/DX11/DX11CommandBuffer.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"

namespace Graphics
{
	namespace DX11
	{
		DX11SwapChain::DX11SwapChain(IDXGIFactory* factory, ID3D11Device* device, const SwapChainDesc& desc)
			: SwapChain()
			, m_Device(device)
		{
			GetWindow().GetWindowInfo()._hwnd = reinterpret_cast<HWND>(desc._windowDesc._hwnd);

			CreateSwapChain(factory, m_Device, desc);

			CreateBackBuffer();

			m_Type = Type::SwapChain;
		}

		DX11SwapChain::~DX11SwapChain()
		{

		}

		void DX11SwapChain::Present()
		{
			auto _error = m_Device->GetDeviceRemovedReason();
			
			switch (_error)
			{
				case S_OK:
					break;
				case DXGI_ERROR_DEVICE_HUNG:
				case DXGI_ERROR_DEVICE_RESET:
					assert(false);
					break;
				case DXGI_ERROR_DEVICE_REMOVED:
				case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
				case DXGI_ERROR_INVALID_CALL:
					assert(false);
					break;
				default:
					break;
			}

			HR(m_Underlying->Present(0, 0), "SwapChain present failed");
		}

		bool DX11SwapChain::SwitchFullscreen(bool enable)
		{
			assert(false);
			return false;
		}

		void DX11SwapChain::BindFramebufferView(DX11CommandBuffer* commandBuffer)
		{
			/* Bind framebuffer of this swap-chain in command buffer */
			if (commandBuffer != nullptr)
				commandBuffer->BindFramebufferView(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

			/* Store reference to last used command buffer */
			m_CommandBuffer = commandBuffer;
		}

		void DX11SwapChain::SetName(const char* name)
		{
			if (name != nullptr)
			{
				DX11SetObjectName(m_BackBuffer.Get(), name);
				DX11SetObjectNameSubscript(m_RenderTargetView.Get(), name, ".RTV");
				if (m_DepthBuffer)
				{
					DX11SetObjectName(m_DepthBuffer.Get(), name);
					DX11SetObjectNameSubscript(m_DepthStencilView.Get(), name, ".DSV");
				}
			}
			else
			{
				/* Reset all back-buffer labels */
				DX11SetObjectName(m_BackBuffer.Get(), nullptr);
				DX11SetObjectName(m_RenderTargetView.Get(), nullptr);
				if (m_DepthBuffer)
				{
					DX11SetObjectName(m_DepthBuffer.Get(), nullptr);
					DX11SetObjectName(m_DepthStencilView.Get(), nullptr);
				}
			}
		}

		bool DX11SwapChain::ResizeBuffer(const Extent2D& resolution)
		{
			ResizeBackBuffer(resolution);
			return true;
		}

		void DX11SwapChain::ResizeBackBuffer(const Extent2D& resolution)
		{
			m_RenderTargetView.Reset();
			m_BackBuffer.Reset();
			m_DepthStencilView.Reset();
			m_DepthBuffer.Reset();

			auto _hr = m_Underlying->ResizeBuffers(1, resolution._width, resolution._height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			HR(_hr, "failed to resize DXGI swap-chain buffers");

			CreateBackBuffer();
		}

		void DX11SwapChain::CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, const SwapChainDesc& desc)
		{
			m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			m_SampleDesc = DX11RenderSystem::FindSuitableSampleDesc(device, DXGI_FORMAT_R8G8B8A8_UNORM, desc._samples);

			auto _window = GetWindow();

			DXGI_SWAP_CHAIN_DESC _desc;
			{
				_desc.BufferDesc.Width = static_cast<uint32>(desc._resolution._width);
				_desc.BufferDesc.Height = static_cast<uint32>(desc._resolution._height);
				_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 디스플레이 비례 모드
				_desc.BufferDesc.RefreshRate.Numerator = 60;
				_desc.BufferDesc.RefreshRate.Denominator = 1;
				_desc.SampleDesc = m_SampleDesc;
				_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				_desc.BufferCount = (desc._swapBuffers >= 3 ? 2 : 1);
				_desc.OutputWindow = _window.GetHwnd();
				_desc.Windowed = true;//(fullscreen ? FALSE : TRUE);
				_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				_desc.Flags = 0;
			}

			HR(factory->CreateSwapChain(device, &_desc, m_Underlying.ReleaseAndGetAddressOf()), "failed to create DXGI swap chain");
		}

		void DX11SwapChain::CreateBackBuffer()
		{
			HR(m_Underlying->GetBuffer(0, IID_PPV_ARGS(m_BackBuffer.ReleaseAndGetAddressOf())), "failed to get D3D11 back buffer from swap chain");
			
			HR(m_Device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, m_RenderTargetView.ReleaseAndGetAddressOf()), "failed to create D3D11 render-target-view (RTV) for back buffer");

			D3D11_TEXTURE2D_DESC _backBufferDesc;

			m_BackBuffer->GetDesc(&_backBufferDesc);

			m_DepthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			if (m_DepthBufferFormat != DXGI_FORMAT_UNKNOWN)
			{
				D3D11_TEXTURE2D_DESC _texDesc;
				{
					_texDesc.Width = _backBufferDesc.Width;
					_texDesc.Height = _backBufferDesc.Height;
					_texDesc.MipLevels = 1;
					_texDesc.ArraySize = 1;
					_texDesc.Format = m_DepthBufferFormat;
					_texDesc.SampleDesc = m_SampleDesc;
					_texDesc.Usage = D3D11_USAGE_DEFAULT;
					_texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					_texDesc.CPUAccessFlags = 0;
					_texDesc.MiscFlags = 0;
				}

				HR(m_Device->CreateTexture2D(&_texDesc, nullptr, m_DepthBuffer.ReleaseAndGetAddressOf()), "failed to create D3D11 depth-texture for swap-chain");

				HR(m_Device->CreateDepthStencilView(m_DepthBuffer.Get(), nullptr, m_DepthStencilView.ReleaseAndGetAddressOf()), "failed to create D3D11 depth-stencil-view (DSV) for swap-chain");
			}
		}
	}
}