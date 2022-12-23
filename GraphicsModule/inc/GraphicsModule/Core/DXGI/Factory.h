#pragma once

#include "Common.h"
#include "GraphicsModule\Core\Common\ComPtr.h"
struct IDXGIDevice;
struct IDXGISwapChain;
struct IDXGIAdapter;
struct IDXGIFactory;

namespace Graphics
{
	class Factory : public std::enable_shared_from_this<Factory>
	{

	public:
		Factory();
		~Factory();

		using UnderlyingType = IDXGIFactory*;

		UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
		UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

	private:
		ComPtr<IDXGIFactory> m_Underlying;
	};
}