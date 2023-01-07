#pragma once

#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

struct ID3D11Device;
struct ID3D11Resource;
struct ID3D11RenderTargetView;
struct D3D11_RENDER_TARGET_VIEW_DESC;

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

			UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

		private:
			ComPtr<ID3D11RenderTargetView> m_Underlying;
			
			std::vector<ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
			std::vector<ID3D11RenderTargetView*> m_RenderTargetViewRefs;

			ID3D11Device* m_pDevice;
			
			static ComPtr<ID3D11RenderTargetView> CreateRenderTarget(ID3D11Device* device, const D3D11_RENDER_TARGET_VIEW_DESC& desc, ID3D11Resource* resource);
		};
	}
}