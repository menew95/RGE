#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11SwapChain.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Device.h"

namespace Graphics
{
	namespace DX11
	{
		DX11SwapChain::DX11SwapChain(IDXGIFactory* factory, ID3D11Device* device, SWAP_CHAIN_DESC& desc)
			: SwapChain()
		{
			CreateSwapChain(factory, device, desc);

			CreateBackBuffer(device);
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

		void DX11SwapChain::CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, SWAP_CHAIN_DESC& desc)
		{
			/* Pick and store color format */
			m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM

			HR(factory->CreateSwapChain(device, (DXGI_SWAP_CHAIN_DESC*)&desc, m_Underlying.ReleaseAndGetAddressOf()));
		}

		void DX11SwapChain::CreateBackBuffer(ID3D11Device* device)
		{
			HR(m_Underlying->GetBuffer(0, IID_PPV_ARGS(m_BackBuffer.ReleaseAndGetAddressOf())));
			
			HR(device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, m_RenderTargetView.ReleaseAndGetAddressOf()));

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

				HR(device->CreateTexture2D(&_texDesc, nullptr, m_DepthBuffer.ReleaseAndGetAddressOf()));

				HR(device->CreateDepthStencilView(m_DepthBuffer.Get(), nullptr, m_DepthStencilView.ReleaseAndGetAddressOf()));
			}
		}
	}
}