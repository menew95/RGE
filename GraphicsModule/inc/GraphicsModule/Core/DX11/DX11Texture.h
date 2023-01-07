#pragma once

#include "GraphicsModule/Core/Texture.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

struct ID3D11Resource;
struct ID3D11Device;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

namespace Graphics
{
	namespace DX11
	{
		union DX11NativeTexture
		{
			inline DX11NativeTexture() : _resource{ nullptr }
			{}

			inline DX11NativeTexture(const DX11NativeTexture& texture) : _resource{ texture._resource }
			{}

			inline DX11NativeTexture& operator=(const DX11NativeTexture& texture)
			{
				_resource = texture._resource;
			}

			inline ~DX11NativeTexture()
			{
				_resource.Reset();
			}

			ComPtr<ID3D11Resource> _resource;
			ComPtr<ID3D11Texture1D> _tex1D;
			ComPtr<ID3D11Texture2D> _tex2D;
			ComPtr<ID3D11Texture3D> _tex3D;
		};

		class DX11Texture : public Texture
		{
		public:
			DX11Texture(ID3D11Device* device, const TextureDesc& desc);
			~DX11Texture() override;


			using UnderlyingType = ID3D11Resource*;

			UnderlyingType GetUnderlying() { return m_NativeTexture._resource.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_NativeTexture._resource.GetAddressOf(); }

			ID3D11ShaderResourceView* GetSRV() { return m_ShaderResourceView.Get(); }
			ID3D11UnorderedAccessView* GetUAV() { return m_UnorderedAccessView.Get(); }
		private:
			uint32 GetTextureMiscFlags(const TextureDesc& desc);

			void CreateTexture1D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture2D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture3D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);

			static ComPtr<ID3D11Texture1D> DXCreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			static ComPtr<ID3D11Texture2D> DXCreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			static ComPtr<ID3D11Texture3D> DXCreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			
			void LoadTexture(ID3D11Device* device, ImageDesc& srcDesc);
			FileFormat CheckFileFormat(const tstring& path);

			DX11NativeTexture m_NativeTexture;

			ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
			ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;

			TextureDesc m_TextureDesc;
		};
	}
}