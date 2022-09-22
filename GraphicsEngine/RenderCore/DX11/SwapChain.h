#pragma once

#include "../ID3D.h"

namespace RenderCore
{
	class SwapChain : public std::enable_shared_from_this<SwapChain>
	{
	public:
		SwapChain();
		~SwapChain();

		shared_ptr<SwapChain> CreateSwapChain(ID3D::Device*, DXGI_SWAP_CHAIN_DESC&);

		void Present();

		void OnResize(ID3D::Device*, uint32 , uint32);

		IDXGI::SwapChain* GetSwapChain() { return mSwapChain.Get(); }
		IDXGI::SwapChain** GetSwapChainRef() { return mSwapChain.GetAddressOf(); }

	private:
		ComPtr<IDXGI::SwapChain> mSwapChain;
		ComPtr<ID3D::RenderTargetView> mRenderTargetView;

	};
}

