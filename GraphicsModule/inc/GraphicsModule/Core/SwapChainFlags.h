#pragma once

#include "GraphicsModule/Core/Format.h"
#include "Math/Math.h"

namespace Graphics
{
	/*struct ModeDesc
	{
		HWND hwnd;
		UINT Width;
		UINT Height;
		DXGI_RATIONAL RefreshRate;
		Format Format;
		DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
		DXGI_MODE_SCALING Scaling;
	};

	struct SampleDesc
	{
		uint32 Count;
		uint32 Quality;
	};*/

	/*struct SwapChainDesc
	{
		ModeDesc _modeDesc;
		SampleDesc _sampleDesc;
		uint32 _BufferUsage;
		uint32 _BufferCount;
		void* _outputWindow;
		bool Windowed;
		uint32 SwapEffect;
		uint32 Flags;

		uint32 _samples = 1;
		uint32 _swapBuffers = 2;
	};*/

	struct WindowDesc
	{
		void* _hwnd;
	};

	struct SwapChainDesc
	{
		bool _fullScreen = false;

		Extent2D _resolution;

		uint32 _samples = 1;

		uint32 _swapBuffers = 2;

		uint32 _colorBit = 32;
		uint32 _depthBit = 24;
		uint32 _stencilBit = 8;

		WindowDesc _windowDesc;
	};
}