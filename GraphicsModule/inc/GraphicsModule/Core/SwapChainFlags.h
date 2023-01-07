#pragma once

namespace Graphics
{
	struct ModeDesc
	{
		HWND hwnd;
		UINT Width;
		UINT Height;
		DXGI_RATIONAL RefreshRate;
		DXGI_FORMAT Format;
		DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
		DXGI_MODE_SCALING Scaling;
	};

	struct SampleDesc
	{
		uint32 Count;
		uint32 Quality;
	};

	struct SwapChainDesc
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
	};
}