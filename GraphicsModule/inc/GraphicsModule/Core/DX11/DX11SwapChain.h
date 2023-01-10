#pragma once

#include "GraphicsModule/Core/SwapChain.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

#include <dxgi.h>

struct SWAP_CHAIN_DESC
{

};

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Graphics
{
	class DeviceContext;
	class Device;

	namespace DX11
	{
		class DX11Device;

		class DX11SwapChain : public SwapChain
		{
		public:
			DX11SwapChain(IDXGIFactory* factory, ID3D11Device* device, const SwapChainDesc& desc);
			~DX11SwapChain() override;

			void Present() override;

			using UnderlyingType = IDXGISwapChain*;

			UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

		private:
			void ResizeBuffer(ID3D11Device*, uint32, uint32);

			// Create SwapChain
			void CreateSwapChain(IDXGIFactory* factory, ID3D11Device* device, const SwapChainDesc& desc);
			void CreateBackBuffer();

			ID3D11Device* m_Device;

			ComPtr<IDXGISwapChain> m_Underlying;
			DXGI_SAMPLE_DESC m_SampleDesc;

			ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
			ComPtr<ID3D11Texture2D> m_BackBuffer;
			DXGI_FORMAT m_BackBufferFormat;

			ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
			ComPtr<ID3D11Texture2D> m_DepthBuffer;
			DXGI_FORMAT m_DepthBufferFormat;
		};
	}
}