#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{

		DX11Texture::DX11Texture(ID3D11Device* device, const TEXTURED_DESC& desc)
		{

		}

		void DX11Texture::CreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr)
		{
			device->CreateTexture1D(&desc, initialData, reinterpret_cast<ID3D11Texture1D**>(m_Underlying.ReleaseAndGetAddressOf()));
		}

		void DX11Texture::CreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			device->CreateTexture2D(&desc, initialData, reinterpret_cast<ID3D11Texture2D**>(m_Underlying.ReleaseAndGetAddressOf()));
		}

		void DX11Texture::CreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			device->CreateTexture3D(&desc, initialData, reinterpret_cast<ID3D11Texture3D**>(m_Underlying.ReleaseAndGetAddressOf()));
		}

	}
}