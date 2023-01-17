#include "GraphicsPCH.h"
#include "GraphicsModule/Core/SwapChain.h"


namespace Graphics
{
    struct SwapChain::Pimpl
    {
        std::shared_ptr<Window> m_Window;
        Extent2D _resolution;
    };

    SwapChain::SwapChain()
        : m_Pimpl(new Pimpl)
    {
        m_Pimpl->m_Window = Window::Create();
    }

    SwapChain::SwapChain(SwapChainDesc& desc)
        : SwapChain {}
    {
        m_Pimpl->_resolution = desc._resolution;
    }

    SwapChain::~SwapChain()
    {
        delete m_Pimpl;
    }

    Window& SwapChain::GetWindow() const
    {
        return *m_Pimpl->m_Window;
    }
}