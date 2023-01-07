#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Device.h"
#include "GraphicsModule/Core/DX11/DX11RenderSystem.h"

namespace Graphics
{
	namespace DX11
	{
		DX11SwapChain::DX11SwapChain(IDXGIFactory* factory, ID3D11Device* device, SwapChainDesc& desc)
			: SwapChain()
			, m_Device(device)
		{
			CreateSwapChain(factory, m_Device, desc);

			CreateBackBuffer();
		}

		DX11SwapChain::~DX11SwapChain()
		{

		}

		void DX11SwapChain::Present()
		{
			HR(m_Underlying->Present(0, 0));
		}

		void DX11SwapChain::ResizeBuffer(DX11Device* device, uint32 width, uint32 height)
		{
			assert(m_Underlying);

			m_Underlying->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			m_Underlying->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_BackBuffer.GetAddressOf()));

			if (m_BackBuffer != nullptr)
			{
				device->GetUnderlying()->CreateRenderTargetView(m_BackBuffer.Get(), 0, m_RenderTargetView.ReleaseAndGetAddressOf());
			}
		}

		void DX11SwapChain::CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, SwapChainDesc& desc)
		{
			/* Pick and store color format */
			m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM

			DXGI_SAMPLE_DESC swapChainSampleDesc_ = DX11RenderSystem::FindSuitableSampleDesc(device, DXGI_FORMAT_R8G8B8A8_UNORM, desc._samples);

			DXGI_SWAP_CHAIN_DESC _desc;
			{
				_desc.BufferDesc.Width = desc._modeDesc.Width;
				_desc.BufferDesc.Height = desc._modeDesc.Height;
				_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				_desc.BufferDesc.RefreshRate.Numerator = 60;
				_desc.BufferDesc.RefreshRate.Denominator = 1;
				_desc.SampleDesc.Count = desc._sampleDesc.Count;
				_desc.SampleDesc.Quality = desc._sampleDesc.Quality;
				_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				_desc.BufferCount = (desc._swapBuffers >= 3 ? 2 : 1);
				_desc.OutputWindow = desc._modeDesc.hwnd;
				_desc.Windowed = TRUE;//(fullscreen ? FALSE : TRUE);
				_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			}

			HR(factory->CreateSwapChain(device, &_desc, m_Underlying.ReleaseAndGetAddressOf()));
		}

		void DX11SwapChain::CreateBackBuffer()
		{
			HR(m_Underlying->GetBuffer(0, IID_PPV_ARGS(m_BackBuffer.ReleaseAndGetAddressOf())));
			
			HR(m_Device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, m_RenderTargetView.ReleaseAndGetAddressOf()));

			D3D11_TEXTURE2D_DESC _backBufferDesc;

			m_BackBuffer->GetDesc(&_backBufferDesc);

			if (m_DepthBufferFormat != DXGI_FORMAT_UNKNOWN)
			{
				/* Create depth stencil texture */
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

				HR(m_Device->CreateTexture2D(&_texDesc, nullptr, m_DepthBuffer.ReleaseAndGetAddressOf()));

				HR(m_Device->CreateDepthStencilView(m_DepthBuffer.Get(), nullptr, m_DepthStencilView.ReleaseAndGetAddressOf()));
			}
		}
	}
}