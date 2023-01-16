#include "GraphicsPCH.h"
#include "GraphicsModule/Core/CommandBuffer.h"
#include "GraphicsModule/Core/DX11/DX11RenderTarget.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"

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
		: m_Device(device)
	{
		CreateRenderTarget(device, desc);
	}

	DX11::DX11RenderTarget::~DX11RenderTarget()
	{

	}

	void DX11::DX11RenderTarget::CreateRenderTarget(ID3D11Device* device, const RenderTargetDesc& desc)
	{

		for (size_t i = 0; i < desc._attachments.size(); i++)
		{
			// todo : 렌더 타겟 타입에 따른 생성
			switch (desc._attachments[i]._renderTargetType)
			{
				case RenderTargetType::RenderTarget:
				{
					CreateRenderTargetView(device, desc._attachments[i]);
					break;
				}
				case RenderTargetType::Depth:
				case RenderTargetType::DepthStencil:
				case RenderTargetType::Stencil:
				{
					CreateDepthStencilView(device, desc._attachments[i]);
					break;
				}
				default:
					assert(false);
					break;
			}
		}
	}

	void DX11::DX11RenderTarget::CreateRenderTargetView(ID3D11Device* device, const AttachmentDesc& desc)
	{
		DX11Texture* _castTex = reinterpret_cast<DX11Texture*>(desc._resource);

		D3D11_RENDER_TARGET_VIEW_DESC _rtvDesc;
		memset(&_rtvDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

		switch (_castTex->GetType())
		{
			case TextureType::Texture1D:
				FillViewDescForTexture1D(desc, _rtvDesc);
				break;
			case TextureType::Texture2D:
				FillViewDescForTexture2D(desc, _rtvDesc);
				break;
			case TextureType::Texture3D:
				FillViewDescForTexture3D(desc, _rtvDesc);
				break;
			case TextureType::Texture1DArray:
				FillViewDescForTexture1DArray(desc, _rtvDesc);
				break;
			case TextureType::TextureCube:
			case TextureType::Texture2DArray:
			case TextureType::TextureCubeArray:
				FillViewDescForTexture2DArray(desc, _rtvDesc);
				break;
			case TextureType::Texture2DMS:
				FillViewDescForTexture2DMS(desc, _rtvDesc);
				break;
			case TextureType::Texture2DMSArray:
				FillViewDescForTexture2DArrayMS(desc, _rtvDesc);
				break;
		}

		ComPtr<ID3D11RenderTargetView> _rtv;
		auto _hr = device->CreateRenderTargetView(_castTex->GetNativeTexture()._resource.Get(), &_rtvDesc, _rtv.ReleaseAndGetAddressOf());

		HR(_hr, "failed to create render-target-view");

		m_RenderTargetViews.push_back(_rtv);
		m_RenderTargetViewRefs.push_back(_rtv.Get());
		m_RTBuffers.push_back(_castTex);
	}

	void DX11::DX11RenderTarget::CreateDepthStencilView(ID3D11Device* device, const AttachmentDesc& desc)
	{
		DX11Texture* _castTex = reinterpret_cast<DX11Texture*>(desc._resource);

		D3D11_DEPTH_STENCIL_VIEW_DESC _dsvDesc;
		memset(&_dsvDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		m_DSVFormat = ToDXGIFormatDSV(MapFormat(_castTex->GetDesc()._format));
		_dsvDesc.Format = m_DSVFormat;
		_dsvDesc.Flags = 0;

		switch (_castTex->GetType())
		{
			case TextureType::Texture1D:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				_dsvDesc.Texture1D.MipSlice = desc._mipLevel;
				break;

			case TextureType::Texture2D:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				_dsvDesc.Texture2D.MipSlice = desc._mipLevel;
				break;

			case TextureType::Texture1DArray:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
				_dsvDesc.Texture1DArray.MipSlice = desc._mipLevel;
				_dsvDesc.Texture1DArray.FirstArraySlice = desc._arrayLayer;
				_dsvDesc.Texture1DArray.ArraySize = 1;
				break;

			case TextureType::Texture3D:
			case TextureType::Texture2DArray:
			case TextureType::TextureCube:
			case TextureType::TextureCubeArray:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				_dsvDesc.Texture2DArray.MipSlice = desc._mipLevel;
				_dsvDesc.Texture2DArray.FirstArraySlice = desc._arrayLayer;
				_dsvDesc.Texture2DArray.ArraySize = 1;
				break;

			case TextureType::Texture2DMS:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				break;

			case TextureType::Texture2DMSArray:
				_dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				_dsvDesc.Texture2DMSArray.FirstArraySlice = desc._arrayLayer;
				_dsvDesc.Texture2DMSArray.ArraySize = 1;
				break;
		}

		auto _hr = device->CreateDepthStencilView(_castTex->GetNativeTexture()._resource.Get(), &_dsvDesc, m_DepthStencilView.ReleaseAndGetAddressOf());

		HR(_hr, "failed to create depth-stencil-view");
		
		m_DSVBuffer = _castTex;
	}

	void DX11::DX11RenderTarget::ClearRenderTarget(ID3D11DeviceContext* context, uint32 numAttachments, const AttachmentClear* attachments)
	{
		for (uint32 i = 0; i < numAttachments; i++)
		{
			if ((attachments[i]._flags & ClearFlags::Color) != 0)
			{

				context->ClearRenderTargetView(
					m_RenderTargetViewRefs[attachments[i]._colorAttachment],
					attachments[i]._clearValue._color
				);
			}
			else if(m_DepthStencilView.Get() != nullptr)
			{
				uint32 _flags = 0;

				if ((attachments[i]._flags & ClearFlags::Depth) != 0)
					_flags |= D3D11_CLEAR_DEPTH;
				if ((attachments[i]._flags & ClearFlags::Stencil) != 0)
					_flags |= D3D11_CLEAR_STENCIL;

				context->ClearDepthStencilView(
					m_DepthStencilView.Get(),
					_flags, attachments->_clearValue._depth,
					static_cast<UINT8>(attachments->_clearValue._stencil & 0xff));
			}
		}
	}
}