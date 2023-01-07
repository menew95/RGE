#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"

#include "GraphicsModule/Core/DirectXTex/DirectXTex.h"

namespace Graphics
{
	namespace DX11
	{

		DX11Texture::DX11Texture(ID3D11Device* device, const TextureDesc& desc)
			: Texture{ desc._textureType, desc._bindFlags }
		{
		}

		DX11Texture::~DX11Texture()
		{

		}

		uint32 DX11Texture::GetTextureMiscFlags(const TextureDesc& desc)
		{
			UINT flagsD3D = 0;

			if (IsMipMappedTexture(desc))
			{
				const long requiredFlags = BindFlags::RenderTarget | BindFlags::ShaderResource;
				const long disallowedFlags = BindFlags::DepthStencil;
				if ((desc._bindFlags & (requiredFlags | disallowedFlags)) == requiredFlags)
					flagsD3D |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}

			if (IsCubeTexture(desc._textureType))
				flagsD3D |= D3D11_RESOURCE_MISC_TEXTURECUBE;

			return flagsD3D;
		}

		void DX11Texture::CreateTexture1D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			D3D11_TEXTURE1D_DESC _desc = { 0 };
			
			_desc.Width = (UINT)desc._extend.x;
			_desc.MipLevels = desc._mipLevels;
			_desc.ArraySize = desc._arrayLayers;
			_desc.Format = MapFormat(desc._format);
			_desc.Usage = D3D11_USAGE_DEFAULT;
			_desc.BindFlags = MapTextureBindFlag(desc._bindFlags);
			_desc.CPUAccessFlags = MapCPUAcessFlagsForMiscFlags(desc._miscFlags);
			_desc.MiscFlags = GetTextureMiscFlags(desc);

			m_NativeTexture._tex1D = DXCreateTexture1D(device, _desc, initialData);
		}

		void DX11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			D3D11_TEXTURE2D_DESC _desc = { 0 };

			_desc.Width = (UINT)desc._extend.x;
			_desc.Height = (UINT)desc._extend.y;
			_desc.MipLevels = desc._mipLevels;
			_desc.ArraySize = desc._arrayLayers;
			_desc.Format = MapFormat(desc._format);
			_desc.SampleDesc.Count = desc._samples;
			_desc.SampleDesc.Quality = 0;
			_desc.Usage = D3D11_USAGE_DEFAULT;
			_desc.BindFlags = MapTextureBindFlag(desc._bindFlags);
			_desc.CPUAccessFlags = MapCPUAcessFlagsForMiscFlags(desc._miscFlags);
			_desc.MiscFlags = GetTextureMiscFlags(desc);

			m_NativeTexture._tex2D = DXCreateTexture2D(device, _desc, initialData);
		}

		void DX11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			D3D11_TEXTURE3D_DESC _desc = { 0 };

			_desc.Width = (UINT)desc._extend.x;
			_desc.Height = (UINT)desc._extend.y;
			_desc.Depth = (UINT)desc._extend.z;
			_desc.MipLevels = desc._mipLevels;
			_desc.Format = MapFormat(desc._format);
			_desc.Usage = D3D11_USAGE_DEFAULT;
			_desc.BindFlags = MapTextureBindFlag(desc._bindFlags);
			_desc.CPUAccessFlags = MapCPUAcessFlagsForMiscFlags(desc._miscFlags);
			_desc.MiscFlags = GetTextureMiscFlags(desc);

			m_NativeTexture._tex3D = DXCreateTexture3D(device, _desc, initialData);
		}

		Graphics::ComPtr<ID3D11Texture1D> DX11Texture::DXCreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture1D> _tex1D;

			HR(device->CreateTexture1D(&desc, initialData, reinterpret_cast<ID3D11Texture1D**>(_tex1D.ReleaseAndGetAddressOf())));

			return _tex1D;
		}

		Graphics::ComPtr<ID3D11Texture2D> DX11Texture::DXCreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture2D> _tex2D;

			HR(device->CreateTexture2D(&desc, initialData, reinterpret_cast<ID3D11Texture2D**>(_tex2D.ReleaseAndGetAddressOf())));

			return _tex2D;
		}

		Graphics::ComPtr<ID3D11Texture3D> DX11Texture::DXCreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture3D> _tex3D;

			HR(device->CreateTexture3D(&desc, initialData, reinterpret_cast<ID3D11Texture3D**>(_tex3D.ReleaseAndGetAddressOf())));

			return _tex3D;
		}

		void DX11Texture::LoadTexture(ID3D11Device* device, ImageDesc& srcDesc)
		{
			using namespace DirectX;

			if (srcDesc._filePath.size() <= 0)
			{
				AssertMessageBox(false, "filePath is not invaild");
			}

			ScratchImage image;

			switch (CheckFileFormat(srcDesc._filePath))
			{
				case FileFormat::DDS:
				{
					LoadFromDDSFile(srcDesc._filePath.c_str(), DDS_FLAGS_NONE, nullptr, image);
					break;
				}
				case FileFormat::TGA:
				{
					LoadFromTGAFile(srcDesc._filePath.c_str(), TGA_FLAGS_NONE, nullptr, image);
					break;
				}
				case FileFormat::HDR:
				{
					LoadFromHDRFile(srcDesc._filePath.c_str(), nullptr, image);
					break;
				}
				case FileFormat::WIC:
				default:
				{
					LoadFromWICFile(srcDesc._filePath.c_str(), WIC_FLAGS_NONE, nullptr, image);
					break;
				}
			}

			HRESULT hr;

			AssertMessageBox(image.GetImageCount() == 0, "D3D11Texture LoadFaile Error");

			bool isCubeMap = image.GetMetadata().IsCubemap();

			if (isCubeMap)
			{
				DirectX::ScratchImage mipChain;

				HR(CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf()));
				
			}
			else
			{
				HR(CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf()));
				
			}

			D3D11_TEXTURE2D_DESC textureDesc;
			m_NativeTexture._tex2D->GetDesc(&textureDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			srvDesc.Format = textureDesc.Format;
			srvDesc.Texture2D.MipLevels = m_TextureDesc._mipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;

			if (isCubeMap)
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				m_TextureDesc._textureType = TextureType::TextureCube;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				m_TextureDesc._textureType = TextureType::Texture2D;
			}

			HR(device->CreateShaderResourceView(m_NativeTexture._resource.Get(), &srvDesc, m_ShaderResourceView.ReleaseAndGetAddressOf()));
			
			m_TextureDesc._format = UnmapFormat(textureDesc.Format);
			m_TextureDesc._extend = { textureDesc.Width, textureDesc.Height, 0 };
			m_TextureDesc._mipLevels = textureDesc.MipLevels;
			m_TextureDesc._bindFlags = textureDesc.BindFlags;
			
		}

		FileFormat DX11Texture::CheckFileFormat(const tstring& path)
		{
			if (path.length() > 1)
			{
				int extensionStartPoint = path.find_last_of('.') + 1;

				auto fileFormat = path.substr(extensionStartPoint);

				if (fileFormat == TEXT("dds") || fileFormat == TEXT("DDS"))
				{
					return FileFormat::DDS;
				}
				else if (fileFormat == TEXT("tga") || fileFormat == TEXT("TGA"))
				{
					return FileFormat::TGA;
				}
				else if (fileFormat == TEXT("hdr") || fileFormat == TEXT("HDR"))
				{
					return FileFormat::HDR;
				}
				else
				{
					return FileFormat::WIC;
				}
			}

			return FileFormat::WIC;
		}

	}
}