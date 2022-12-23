#pragma once

#include "GraphicsModule/Core/Texture.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"


struct ID3D11Resource;
struct ID3D11Device;
struct TEXTURED_DESC;

namespace Graphics
{
	namespace DX11
	{
		class DX11Texture : public Texture
		{
		public:
			DX11Texture(ID3D11Device* device, const TEXTURED_DESC& desc);
			~DX11Texture() override;


			using UnderlyingType = ID3D11Resource*;

			UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

		private:
			void CreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);

			ComPtr<ID3D11Resource> m_Underlying;
			
		};
	}
}