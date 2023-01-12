#pragma once

#include "GraphicsModule/Window.h"

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/SwapChainFlags.h"


struct IDXGIFactory;

namespace Graphics
{
	class SwapChain
	{
	public:
		SwapChain() = default;
		SwapChain(IDXGIFactory* factory, SwapChainDesc& desc) {}
		virtual ~SwapChain() {}

		virtual void Present() abstract;

		bool ResizeBuffers(const Extent2D& resolution, long flags = 0);

		bool SwitchFullscreen(bool enable);
	};
}