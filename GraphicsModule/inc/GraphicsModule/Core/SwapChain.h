#ifndef MT_SWAPCHAIN_H
#define MT_SWAPCHAIN_H

#include "Common.h"
#include "GraphicsModule/Core/SwapChainFlags.h"

struct IDXGIFactory;

namespace Graphics
{
	struct SWAP_CHAIN_DESC;
	
	class Device;

	class SwapChain
	{
	public:
		SwapChain() = default;
		SwapChain(IDXGIFactory* factory, SwapChainDesc& desc) {}
		virtual ~SwapChain() {}

		virtual void Present() abstract;

		//virtual void OnResize(uint32, uint32) abstract;
	};
}
#endif