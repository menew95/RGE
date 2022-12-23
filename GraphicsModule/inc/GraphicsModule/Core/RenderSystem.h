#pragma once

#include "GraphicsModule/Core/Device.h"
#include "GraphicsModule/Core/DeviceContext.h"
#include "GraphicsModule/Core/SwapChain.h"
#include "GraphicsModule/Core/RenderPipLine.h"
#pragma once

#include "GraphicsModule/Core/DeviceContext.h"
#include "GraphicsModule/Core/Device.h"
#include "GraphicsModule/Core/SwapChain.h"

namespace Graphics
{
	class Device;
	class DeviceContext;
	class SwapChain;

	class Buffer;

	class ShaderResourceView;
	class RenderTargetView;
	class UnorderedAccessView;
	class ConstantBufferView;
	class DepthStencilView;

	class RenderSystem
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();

		virtual void* CreateDeviceContext();
		virtual void* CreateSwapChain();
		virtual void* CreateBuffer();
		virtual void* CreateShaderResourceView();
		virtual void* CreateRenderTargetView();
		virtual void* CreateUnorderedAccessView();
		virtual void* CreateConstantBufferView();
		virtual void* CreateDepthStencilView();

	private:
		virtual void CreateFactory();
	};
}