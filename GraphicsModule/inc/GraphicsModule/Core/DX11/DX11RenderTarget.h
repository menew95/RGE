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
	class DX11Texture;

	namespace DX11
	{

		class DX11RenderTarget : public RenderTarget 
		{
		public:
			DX11RenderTarget(ID3D11Device* device, const RenderTargetDesc& desc);
			~DX11RenderTarget() override;


			using UnderlyingType = ID3D11RenderTargetView*;

			inline const std::vector<ID3D11RenderTargetView*>& GetRenderTargetViews() const { return m_RenderTargetViewRefs; }
			inline ID3D11DepthStencilView* GetDepthStencilView() const { return m_DepthStencilView.Get(); }
			RenderTargetDesc GetDesc() { return m_RenderTargetDesc; }

			void CreateRenderTargetViews(ID3D11Device* device, RenderTargetDesc& desc);
			void CreateDepthStencil(ID3D11Device* device, RenderTargetDesc& desc);

			void ClearRenderTarget(ID3D11DeviceContext* context, struct ClearValue& value);

		private:
			void Attach(const AttachmentDesc& desc);

			ID3D11Device* m_Device;

			RenderTargetDesc m_RenderTargetDesc;

			std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
			std::vector<ID3D11RenderTargetView*> m_RenderTargetViewRefs;
			std::vector<DX11Texture*> m_RTBuffers;

			ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
			DX11Texture* m_DSVBuffer;
			DXGI_FORMAT m_DSVFormat;
			
			void CreateRenderTargetView(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc);
			void CreateDepthStencilView(DXGI_FORMAT format);

			static ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Device* device, const D3D11_RENDER_TARGET_VIEW_DESC& desc, ID3D11Resource* resource);
			static ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(ID3D11Device* device, const D3D11_DEPTH_STENCIL_VIEW_DESC& desc, ID3D11Resource* resource);
		};
	}
}