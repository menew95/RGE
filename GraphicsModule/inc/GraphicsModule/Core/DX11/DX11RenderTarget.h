#pragma once

#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

struct ID3D11Device;
struct ID3D11Resource;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;

namespace Graphics
{

	namespace DX11
	{
		class DX11Texture;

		class DX11RenderTarget : public RenderTarget 
		{
		public:
			DX11RenderTarget(ID3D11Device* device, const RenderTargetDesc& desc);
			~DX11RenderTarget() override;

			using UnderlyingType = ID3D11RenderTargetView*;

			inline const std::vector<ID3D11RenderTargetView*>& GetRenderTargetViews() const { return m_RenderTargetViewRefs; }

			inline const std::vector<DX11Texture*>& GetRYVBuffers() const { return m_RTBuffers; }

			inline ID3D11DepthStencilView* GetDepthStencilView() const { return m_DepthStencilView.Get(); }

			inline const DX11Texture* GetDSVTexture() const { return m_DSVBuffer; }
			
			RenderTargetDesc GetDesc() { return m_RenderTargetDesc; }


			void ClearRenderTarget(ID3D11DeviceContext* context, uint32 numAttachments, const AttachmentClear* attachments);


		private:
			void CreateRenderTarget(ID3D11Device* device, const RenderTargetDesc& desc);
			void CreateRenderTargetView(ID3D11Device* device, const AttachmentDesc& desc);
			void CreateDepthStencilView(ID3D11Device* device, const AttachmentDesc& desc);

			ID3D11Device* m_Device = nullptr;

			RenderTargetDesc m_RenderTargetDesc;

			std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
			std::vector<ID3D11RenderTargetView*> m_RenderTargetViewRefs;
			std::vector<DX11Texture*> m_RTBuffers;

			ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
			DX11Texture* m_DSVBuffer = nullptr;
			DXGI_FORMAT m_DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		};
	}
}