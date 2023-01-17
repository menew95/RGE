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
		assert(false);
	}

	std::shared_ptr<Window> Window::Create()
	{
		return std::make_shared<Window>();
	}
}