#pragma once

#include "Common.h"

#include "GraphicsModule/Window.h"

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/SwapChainFlags.h"

#include "GraphicsModule/Core/RenderTarget.h"

struct IDXGIFactory;

namespace Graphics
{
	class SwapChain : public RenderTarget
	{
	public:
		SwapChain();
		SwapChain(SwapChainDesc& desc);
		virtual ~SwapChain();

		virtual void Present() abstract;

		virtual bool ResizeBuffer(const Extent2D& resolution) abstract;

		virtual bool SwitchFullscreen(bool enable) abstract;

		Window& GetWindow() const;

	private:
		struct Pimpl;
		Pimpl* m_Pimpl;
	};
}