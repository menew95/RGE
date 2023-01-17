#pragma once

#include "Common.h"

namespace Graphics
{
	struct WindowInfo
	{
		uint32 _refrashRate = 60;
		HWND _hwnd = 0;
	};

	class Window
	{
	public:
		Window();
		~Window();


		void OnResize(uint32 width, uint32 height);

		inline uint32 GetRefrashRate() { return s_WindowInfo._refrashRate; }
		inline HWND GetHwnd() { return s_WindowInfo._hwnd; }

		inline WindowInfo& GetWindowInfo() { return s_WindowInfo; }
		 
		static std::shared_ptr<Window> Create();

	private:
		WindowInfo s_WindowInfo;
	};
}