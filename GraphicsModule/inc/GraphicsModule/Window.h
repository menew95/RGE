#pragma once

#include "Common.h"

namespace Graphics
{
	struct WindowInfo
	{
		uint32 _height = 1280;
		uint32 _width = 720;

		uint32 _refrashRate = 60;
		HWND _hwnd = 0;
	};

	class Window
	{
	public:
		Window();
		~Window();


		void OnResize(uint32 width, uint32 height);


		inline static uint32 GetHeight() { return s_WindowInfo._height; }
		inline static uint32 GetWidth() { return s_WindowInfo._width; }
		inline static uint32 GetRefrashRate() { return s_WindowInfo._refrashRate; }
		inline static HWND GetHwnd() { return s_WindowInfo._hwnd; }

		inline static WindowInfo& GetWindowInfo() { return s_WindowInfo; }

	private:
		static WindowInfo s_WindowInfo;
	};
}