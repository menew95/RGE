#pragma once

#include "Common.h"

namespace Graphics
{
	class Window
	{
	public:
		Window();
		~Window();

		static Window* Windo

		void OnResize(uint32 width, uint32 height);

		uint32 GetHeight() { return m_Height; }
		uint32 GetWidth() { return m_Width; }

		HWND GetHwnd() { return m_Hwnd; }
	private:
		uint32 m_Height;
		uint32 m_Width;

		uint32 m_RefrashRate;
		HWND m_Hwnd;
	};
}