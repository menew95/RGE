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
			DX11Texture(const TextureDesc& desc);
			//DX11Texture(ID3D11Device* device, const TextureDesc& desc);
			~DX11Texture() override;

			inline const DX11NativeTexture& GetNativeTexture() const { return m_NativeTexture; }
			inline const ID3D11Resource* GetResource() const { return m_NativeTexture._resource.Get(); }
			inline ID3D11ShaderResourceView* GetSRV() const { return m_ShaderResourceView.Get(); }
			inline ID3D11UnorderedAccessView* GetUAV() const { return m_UnorderedAccessView.Get(); }

			inline auto& GetDesc() { return m_TextureDesc; }
			TextureType GetType() { return m_TextureDesc._textureType; }

			void CreateTexture1D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture2D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			void CreateTexture3D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);

			void UpdateSubresource(
				ID3D11DeviceContext* context,
				UINT                        mipLevel,
				UINT                        arrayLayer,
				const D3D11_BOX& region,
				const ImageDesc& imageDesc
			);

			void CreateSubresourceCopyWithCPUAccess(
				ID3D11Device* device,
				ID3D11DeviceContext* context,
				DX11NativeTexture& textureOutput,
				UINT                    cpuAccessFlags,
				const TextureRegion& region
			);

			void CreateShaderResourceView(ID3D11Device* device, uint32 baseMipLevel, uint32 numMipLevels, uint32 baseArrayLayer, uint32 numArrayLayers);
			void CreateUnorderedAccessView(ID3D11Device* device, uint32 baseMipLevel, uint32 numMipLevels, uint32 baseArrayLayer, uint32 numArrayLayers);
			//void CreateUnorderedAccessView(ID3D11Device* device, ID3D11Resource* resource, ID3D11UnorderedAccessView** uavOutput, const TextureType type, const DXGI_FORMAT format, uint32 baseMipLevel, uint32 baseArrayLayer, uint32 numArrayLayers, const char* errorContextInfo = nullptr);

			void CreateTextureFromFile(ID3D11Device* device, const ImageDesc& srcDesc);

			inline DXGI_FORMAT GetDXFormat() const
			{
				return m_Format;
			}

			inline uint32 GetNumMipLevels() const
			{
				return m_NumMipLevels;
			}

			inline uint32 GetNumArrayLayers() const
			{
				return m_NumArrayLayers;
			}

		private:
			void SetResourceParams(DXGI_FORMAT format, const Extent3D& extent, UINT mipLevels, UINT arraySize);

			uint32 GetTextureMiscFlags(const TextureDesc& desc);


			static ComPtr<ID3D11Texture1D> DXCreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			static ComPtr<ID3D11Texture2D> DXCreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			static ComPtr<ID3D11Texture3D> DXCreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData = nullptr);
			
			FileFormat CheckFileFormat(const tstring& path);

			DX11NativeTexture m_NativeTexture;

			ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
			ComPtr<ID3D11UnorderedAccessView> m_UnorderedAccessView;

			TextureDesc m_TextureDesc;
			DXGI_FORMAT m_Format;
			uint32      m_NumMipLevels = 0;
			uint32      m_NumArrayLayers = 0;
		};
	}
}