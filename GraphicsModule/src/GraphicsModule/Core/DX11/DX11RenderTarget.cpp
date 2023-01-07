#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11RenderTarget.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{

	DX11::DX11RenderTarget::DX11RenderTarget(ID3D11Device* device, const RenderTargetDesc& desc)
	{

	}

	DX11::DX11RenderTarget::~DX11RenderTarget()
	{

	}

	Graphics::ComPtr<ID3D11RenderTargetView> DX11::DX11RenderTarget::CreateRenderTarget(ID3D11Device* device, const D3D11_RENDER_TARGET_VIEW_DESC& desc, ID3D11Resource* resource)
	{
		ComPtr<ID3D11RenderTargetView> _rtv;

		HR(device->CreateRenderTargetView(resource, &desc, _rtv.ReleaseAndGetAddressOf()));

		return _rtv;
	}

}