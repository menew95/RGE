#ifndef MT_SWAPCHAIN_H
#define MT_SWAPCHAIN_H

#include "Common.h"

namespace Graphics
{
	struct SWAP_CHAIN_DESC;
	
	class Device;

	class SwapChain
	{
	public:
		SwapChain();
		SwapChain(IDXGIFactory* factory, ID3D11Device* device, SWAP_CHAIN_DESC& desc);
		SwapChain(Device* device, SWAP_CHAIN_DESC& desc);
		virtual ~SwapChain();

		virtual void Present();

		virtual void OnResize(Device*, uint32, uint32);
	};
}
#endif