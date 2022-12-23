#include "GraphicsPCH.h"
#include "GraphicsModule/Window.h"

namespace Graphics
{

	Window::Window()
	{

	}

	Window::~Window()
	{

	}

	void Window::OnResize(uint32 width, uint32 height)
	{
		m_Width = width;
		m_Height = height;
	}

}