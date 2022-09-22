#include "../../pch.h"
#include "IncludeDX11.h"
#include "SwapChain.h"

RenderCore::SwapChain::SwapChain()
{

}

RenderCore::SwapChain::~SwapChain()
{

}

std::shared_ptr<RenderCore::SwapChain> RenderCore::SwapChain::CreateSwapChain(ID3D::Device* device, DXGI_SWAP_CHAIN_DESC& desc)
{
	ComPtr<IDXGI::Device> _dxgiDevice = nullptr;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)_dxgiDevice.GetAddressOf());

	ComPtr<IDXGI::Adapter> _dxgiAdapter = nullptr;
	_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)_dxgiAdapter.GetAddressOf());

	ComPtr<IDXGI::Factory> _dxgiFactory = nullptr;
	_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)_dxgiFactory.GetAddressOf());

	if (_dxgiFactory != nullptr)
	{
		_dxgiFactory->CreateSwapChain(device, &desc, mSwapChain.GetAddressOf());
	}


	return this->shared_from_this();
}

void RenderCore::SwapChain::Present()
{
	HR(mSwapChain->Present(0, 0));
}

void RenderCore::SwapChain::OnResize(ID3D::Device* device, uint32 width, uint32 height)
{
	assert(mSwapChain);

	mSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ComPtr<ID3D::Texture2D> backBuffer = nullptr;
	
	mSwapChain->GetBuffer(0, __uuidof(ID3D::Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (backBuffer != nullptr)
	{
		device->CreateRenderTargetView(backBuffer.Get(), 0, mRenderTargetView.ReleaseAndGetAddressOf());
	}
}
