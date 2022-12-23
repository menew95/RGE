#pragma once

#include "Common.h"

namespace Graphics
{
	class Window
	{
	public:
		Window();
		~Window();

		void OnResize(uint32 width, uint32 height);

		uint32 GetHeight() { return m_Height; }
		uint32 GetWidth() { return m_Width; }
		uint32 GetWidth() { return m_Width; }

	private:
		uint32 m_Height;
		uint32 m_Width;

		uint32 m_RefrashRate;
	};
}