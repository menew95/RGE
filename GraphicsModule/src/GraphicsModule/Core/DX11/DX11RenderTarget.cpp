#include "GraphicsPCH.h"
#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/DX11/DX11RenderTarget.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"

namespace Graphics
{
	static void FillViewDescForTexture1D(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipSlice = attachmentDesc._mipLevel;
	}

	static void FillViewDescForTexture2D(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = attachmentDesc._mipLevel;
	}

	static void FillViewDescForTexture3D(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
		viewDesc.Texture3D.MipSlice = attachmentDesc._mipLevel;
		viewDesc.Texture3D.FirstWSlice = attachmentDesc._arrayLayer;
		viewDesc.Texture3D.WSize = 1;
	}

	static void FillViewDescForTexture1DArray(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
		viewDesc.Texture1DArray.MipSlice = attachmentDesc._mipLevel;
		viewDesc.Texture1DArray.FirstArraySlice = attachmentDesc._arrayLayer;
		viewDesc.Texture1DArray.ArraySize = 1;
	}

	static void FillViewDescForTexture2DArray(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		viewDesc.Texture2DArray.MipSlice = attachmentDesc._mipLevel;
		viewDesc.Texture2DArray.FirstArraySlice = attachmentDesc._arrayLayer;
		viewDesc.Texture2DArray.ArraySize = 1;
	}

	static void FillViewDescForTexture2DMS(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	}

	static void FillViewDescForTexture2DArrayMS(const AttachmentDesc& attachmentDesc, D3D11_RENDER_TARGET_VIEW_DESC& viewDesc)
	{
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
		viewDesc.Texture2DMSArray.FirstArraySlice = attachmentDesc._arrayLayer;
		viewDesc.Texture2DMSArray.ArraySize = 1;
	}

	DX11::DX11RenderTarget::DX11RenderTarget(ID3D11Device* device, const RenderTargetDesc& desc)
	{

	}

	DX11::DX11RenderTarget::~DX11RenderTarget()
	{

	}

	void DX11::DX11RenderTarget::CreateRenderTargetViews(ID3D11Device* device, RenderTargetDesc& desc)
	{

		for (size_t i = 0; i < desc._attachments.size(); i++)
		{
			DX11Texture* _texture = reinterpret_cast<DX11Texture*>(desc._attachments[i]._resource);

			D3D11_RENDER_TARGET_VIEW_DESC _desc;

			switch (_texture->GetType())
			{
				case TextureType::Texture2D:
					FillViewDescForTexture2DMS(desc._attachments[i], _desc);
					break;
				case TextureType::TextureCube:
				case TextureType::Texture2DArray:
				case TextureType::TextureCubeArray:
					FillViewDescForTexture2DArrayMS(desc._attachments[i], _desc);
					break;
				default:
					throw std::invalid_argument("failed to attach D3D11 texture to multi-sample render-target");
					break;
			}

			// todo : ·»´õ Å¸°Ù Å¸ÀÔ¿¡ µû¸¥ »ý¼º
			switch (desc._attachments[i]._renderTargetTyep)
			{
				case RenderTargetType::RenderTarget:
				{
					//auto test = CreateRenderTargetView(device, _desc, _texture->GetResource());

					//m_RenderTargetViews.push_back(CreateRenderTargetView(device, _desc, _texture->GetResource()));
					break;
				}
				case RenderTargetType::Depth:
				{
					break;
				}
				case RenderTargetType::DepthStencil:
				{
					break;
				}
				case RenderTargetType::Stencil:
				{
					break;
				}
				default:
					break;
			}
		}


	}

	void DX11::DX11RenderTarget::ClearRenderTarget(ID3D11DeviceContext* context, struct ClearValue& value)
	{

	}

	void DX11::DX11RenderTarget::Attach(const AttachmentDesc& desc)
	{
		
	}

	ComPtr<ID3D11RenderTargetView> DX11::DX11RenderTarget::CreateRenderTargetView(ID3D11Device* device, const D3D11_RENDER_TARGET_VIEW_DESC& desc, ID3D11Resource* resource)
	{
		ComPtr<ID3D11RenderTargetView> _rtv;

		HR(device->CreateRenderTargetView(resource, &desc, _rtv.ReleaseAndGetAddressOf()), "failed to create render target view");

		return _rtv;
	}

	void DX11::DX11RenderTarget::CreateDepthStencilView(DXGI_FORMAT format)
	{
		/* Create depth-stencil resource */
		D3D11_TEXTURE2D_DESC texDesc;
		{
			texDesc.Width = m_RenderTargetDesc._extend._width;
			texDesc.Height = m_RenderTargetDesc._extend._height;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = format;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;
		}
		// todo : µª½º ½ºÅÙ½Ç 

		//m_Device->CreateTexture2D(&texDesc, nullptr, m_DepthStencilViewBuffer.ReleaseAndGetAddressOf());

		/* Create DSV */
		//m_Device->CreateDepthStencilView(m_DepthStencilViewBuffer.Get(), nullptr, m_DepthStencilView.ReleaseAndGetAddressOf());

		/* Store native depth-stencil format */
		m_DSVFormat = format;
	}

	ComPtr<ID3D11DepthStencilView> DX11::DX11RenderTarget::CreateDepthStencilView(ID3D11Device* device, const D3D11_DEPTH_STENCIL_VIEW_DESC& desc, ID3D11Resource* resource)
	{
		ComPtr<ID3D11DepthStencilView> _dsv;

		HR(device->CreateDepthStencilView(resource, &desc, _dsv.ReleaseAndGetAddressOf()), "failed to create depth stencil view");

		return _dsv;
	}


	void DX11::DX11RenderTarget::CreateRenderTargetView(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc)
	{
		ComPtr<ID3D11RenderTargetView> _rtv;

		HR(m_Device->CreateRenderTargetView(resource, &desc, _rtv.ReleaseAndGetAddressOf()), "failed to create render target view");

		m_RenderTargetViews.push_back(_rtv);
		m_RenderTargetViewRefs.push_back(_rtv.Get());
	}

}