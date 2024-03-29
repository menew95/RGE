﻿#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"
#include "GraphicsModule/Core/DirectXTex/DirectXTex.h"

namespace Graphics
{
	namespace DX11
	{

		DX11Texture::DX11Texture(const TextureDesc& desc)
			: Texture{ desc._textureType, desc._bindFlags }
			, m_TextureDesc(desc)
			, m_Format(DXGI_FORMAT_UNKNOWN)
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

			_desc.Width = desc._extend._width;
			_desc.MipLevels = desc._mipLevels;
			_desc.ArraySize = desc._arrayLayers;
			_desc.Format = MapFormat(desc._format);
			_desc.Usage = D3D11_USAGE_DEFAULT;
			_desc.BindFlags = MapTextureBindFlag(desc._bindFlags);
			_desc.CPUAccessFlags = MapCPUAcessFlagsForMiscFlags(desc._miscFlags);
			_desc.MiscFlags = GetTextureMiscFlags(desc);

			m_NativeTexture._tex1D = DXCreateTexture1D(device, _desc, initialData);

			SetResourceParams(_desc.Format, Extent3D{ _desc.Width, 1u, 1u }, _desc.MipLevels, _desc.ArraySize);
		}

		void DX11Texture::CreateTexture2D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			D3D11_TEXTURE2D_DESC _desc = { 0 };

			_desc.Width = desc._extend._width;
			_desc.Height = desc._extend._height;
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

			SetResourceParams(_desc.Format, Extent3D{ _desc.Width, _desc.Height, 1u }, _desc.MipLevels, _desc.ArraySize);
		}

		void DX11Texture::CreateTexture3D(ID3D11Device* device, const TextureDesc& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			D3D11_TEXTURE3D_DESC _desc = { 0 };

			_desc.Width = desc._extend._width;
			_desc.Height = desc._extend._height;
			_desc.Depth = desc._extend._depth;
			_desc.MipLevels = desc._mipLevels;
			_desc.Format = MapFormat(desc._format);
			_desc.Usage = D3D11_USAGE_DEFAULT;
			_desc.BindFlags = MapTextureBindFlag(desc._bindFlags);
			_desc.CPUAccessFlags = MapCPUAcessFlagsForMiscFlags(desc._miscFlags);
			_desc.MiscFlags = GetTextureMiscFlags(desc);

			m_NativeTexture._tex3D = DXCreateTexture3D(device, _desc, initialData);

			SetResourceParams(_desc.Format, Extent3D{ _desc.Width, _desc.Height, _desc.Depth }, _desc.MipLevels, 1);
		}

		void DX11Texture::SetResourceParams(DXGI_FORMAT format, const Extent3D& extent, UINT mipLevels, UINT arraySize)
		{
			m_Format = format;
			m_NumMipLevels = (mipLevels == 0 ? NumMipLevels(extent._width, extent._height, extent._depth) : mipLevels);
			m_NumArrayLayers = arraySize;
		}

		Graphics::ComPtr<ID3D11Texture1D> DX11Texture::DXCreateTexture1D(ID3D11Device* device, const D3D11_TEXTURE1D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture1D> _tex1D;

			HR(device->CreateTexture1D(&desc, initialData, reinterpret_cast<ID3D11Texture1D**>(_tex1D.ReleaseAndGetAddressOf()))
			, "failed to create D3D11 1D texture");

			return _tex1D;
		}

		Graphics::ComPtr<ID3D11Texture2D> DX11Texture::DXCreateTexture2D(ID3D11Device* device, const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture2D> _tex2D;

			HR(device->CreateTexture2D(&desc, initialData, reinterpret_cast<ID3D11Texture2D**>(_tex2D.ReleaseAndGetAddressOf()))
			, "failed to create D3D11 2D texture");

			return _tex2D;
		}

		Graphics::ComPtr<ID3D11Texture3D> DX11Texture::DXCreateTexture3D(ID3D11Device* device, const D3D11_TEXTURE3D_DESC& desc, const D3D11_SUBRESOURCE_DATA* initialData /*= nullptr*/)
		{
			ComPtr<ID3D11Texture3D> _tex3D;

			HR(device->CreateTexture3D(&desc, initialData, reinterpret_cast<ID3D11Texture3D**>(_tex3D.ReleaseAndGetAddressOf()))
			, "failed to create D3D11 3D texture");

			return _tex3D;
		}

		void DX11Texture::UpdateSubresource(ID3D11DeviceContext* context, UINT mipLevel, UINT arrayLayer, const D3D11_BOX& region, const ImageDesc& imageDesc)
		{
			/* Check if source image must be converted */
			auto format = UnmapFormat(m_Format);


			const void* initialData = imageDesc._data;

			/* Update subresource with specified image data */
			context->UpdateSubresource(
				m_NativeTexture._resource.Get(),
				0,
				&region,
				initialData,
				0,
				0
			);
		}

		void DX11Texture::CreateSubresourceCopyWithCPUAccess(ID3D11Device* device, ID3D11DeviceContext* context, DX11NativeTexture& textureOutput, uint32 cpuAccessFlags, const TextureRegion& region)
		{
			D3D11_RESOURCE_DIMENSION _dimension;
			m_NativeTexture._resource->GetType(&_dimension);

			switch (_dimension)
			{
				case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
				{
					D3D11_TEXTURE1D_DESC _desc;
					m_NativeTexture._tex1D->GetDesc(&_desc);
					{
						_desc.Width = region.extent._width;
						_desc.MipLevels = 1;
						_desc.ArraySize = region.subresource.numArrayLayers;
						_desc.Usage = D3D11_USAGE_STAGING;
						_desc.BindFlags = 0;
						_desc.CPUAccessFlags = cpuAccessFlags;
						_desc.MiscFlags = 0;
					}

					textureOutput._tex1D = DXCreateTexture1D(device, _desc);
					break;
				}
				case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
				{
					D3D11_TEXTURE2D_DESC desc;
					m_NativeTexture._tex2D->GetDesc(&desc);
					{
						desc.Width = region.extent._width;
						desc.Height = region.extent._height;
						desc.MipLevels = 1;
						desc.ArraySize = region.subresource.numArrayLayers;
						desc.Usage = D3D11_USAGE_STAGING;
						desc.BindFlags = 0;
						desc.CPUAccessFlags = cpuAccessFlags;
						desc.MiscFlags = (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE);
					}
					textureOutput._tex2D = DXCreateTexture2D(device, desc);
					break;
				}
				case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
				{
					D3D11_TEXTURE3D_DESC desc;
					m_NativeTexture._tex3D->GetDesc(&desc);
					{
						desc.Width = region.extent._width;
						desc.Height = region.extent._height;
						desc.Depth = region.extent._depth;
						desc.MipLevels = 1;
						desc.Usage = D3D11_USAGE_STAGING;
						desc.BindFlags = 0;
						desc.CPUAccessFlags = cpuAccessFlags;
						desc.MiscFlags = 0;
					}
					textureOutput._tex3D = DXCreateTexture3D(device, desc);
					break;
				}
			}

			const uint32 _mipLevel = region.subresource.baseMipLevel;

			const D3D11_BOX _srcBox
			{
				static_cast<uint32>(region.offset.x),
				static_cast<uint32>(region.offset.y),
				static_cast<uint32>(region.offset.z),
				static_cast<uint32>(region.offset.x) + region.extent._width,
				static_cast<uint32>(region.offset.y) + region.extent._height,
				static_cast<uint32>(region.offset.z) + region.extent._depth,
			};

			for (std::uint32_t i = 0; i < region.subresource.numArrayLayers; ++i)
			{
				const UINT arrayLayer = region.subresource.baseArrayLayer + i;
				context->CopySubresourceRegion(
					textureOutput._resource.Get(),
					D3D11CalcSubresource(0, i, 1),
					0, // DstX
					0, // DstY
					0, // DstZ
					m_NativeTexture._resource.Get(),
					D3D11CalcSubresource(_mipLevel, arrayLayer, m_NumMipLevels),
					&_srcBox
				);
			}
		}

		void DX11Texture::CreateSubresourceCopyWithUIntFormat(ID3D11Device* device, DX11NativeTexture& textureOutput, ID3D11ShaderResourceView** srvOutput, ID3D11UnorderedAccessView** uavOutput, const TextureRegion& region, const TextureType subresourceType)
		{
			uint32 _bindFlags = 0;
			if (srvOutput != nullptr)
				_bindFlags |= D3D11_BIND_SHADER_RESOURCE;

			if (uavOutput != nullptr)
				_bindFlags |= D3D11_BIND_UNORDERED_ACCESS;

			D3D11_RESOURCE_DIMENSION _dimension = D3D11_RESOURCE_DIMENSION_UNKNOWN;
			DXGI_FORMAT _format = DXGI_FORMAT_UNKNOWN;
			uint32 _arraySize = 1;

			m_NativeTexture._resource->GetType(&_dimension);

			switch (_dimension)
			{
				case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
				{
					D3D11_TEXTURE1D_DESC _desc;
					m_NativeTexture._tex1D->GetDesc(&_desc);
					_format = ToDXGIFormatUINT(_desc.Format);
					_arraySize = _desc.ArraySize;
					{
						_desc.Width = region.extent._width;
						_desc.MipLevels = 1;
						_desc.ArraySize = region.subresource.numArrayLayers;
						_desc.Format = _format;
						_desc.Usage = D3D11_USAGE_DEFAULT;
						_desc.BindFlags = _bindFlags;
						_desc.CPUAccessFlags = 0;
						_desc.MiscFlags = 0;
					}
					textureOutput._tex1D = DXCreateTexture1D(device, _desc);
					break;
				}
				case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
				{
					D3D11_TEXTURE2D_DESC _desc;
					m_NativeTexture._tex2D->GetDesc(&_desc);
					_format = ToDXGIFormatUINT(_desc.Format);
					_arraySize = _desc.ArraySize;
					{
						_desc.Width = region.extent._width;
						_desc.Height = region.extent._height;
						_desc.MipLevels = 1;
						_desc.ArraySize = region.subresource.numArrayLayers;
						_desc.Format = _format;
						_desc.Usage = D3D11_USAGE_DEFAULT;
						_desc.BindFlags = _bindFlags;
						_desc.CPUAccessFlags = 0;
						_desc.MiscFlags = (_desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE);
					}
					textureOutput._tex2D = DXCreateTexture2D(device, _desc);
					break;
				}
				case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
				{
					D3D11_TEXTURE3D_DESC _desc;
					m_NativeTexture._tex3D->GetDesc(&_desc);
					_format = ToDXGIFormatUINT(_desc.Format);
					{
						_desc.Width = region.extent._width;
						_desc.Height = region.extent._height;
						_desc.Depth = region.extent._depth;
						_desc.MipLevels = 1;
						_desc.Format = _format;
						_desc.Usage = D3D11_USAGE_DEFAULT;
						_desc.BindFlags = _bindFlags;
						_desc.CPUAccessFlags = 0;
						_desc.MiscFlags = 0;
					}
					textureOutput._tex3D = DXCreateTexture3D(device, _desc);
					break;
				}
			}

			if (srvOutput != nullptr)
			{
				CreateSubresourceSRV
				(
					device,
					textureOutput._resource.Get(),
					srvOutput,
					subresourceType,
					_format,
					0,
					1,
					0,
					_arraySize
				);
			}

			if (uavOutput != nullptr)
			{
				CreateSubresourceUAV
				(
					device,
					textureOutput._resource.Get(),
					uavOutput,
					subresourceType,
					_format,
					0,
					0,
					_arraySize
				);
			}
		}

		void DX11Texture::CreateShaderResourceView(ID3D11Device* device, uint32 baseMipLevel, uint32 numMipLevels, uint32 baseArrayLayer, uint32 numArrayLayers)
		{
			CreateSubresourceSRV(
				device,
				m_NativeTexture._resource.Get(),
				m_ShaderResourceView.GetAddressOf(),
				m_TextureDesc._textureType,
				m_Format,
				baseMipLevel,
				numMipLevels,
				baseArrayLayer,
				numArrayLayers);
		}

		void DX11Texture::CreateSubresourceSRV(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv, const TextureType type, const DXGI_FORMAT format, uint32 baseMipLevel, uint32 numMipLevels, uint32 baseArrayLayer, uint32 numArrayLayers)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC _desc;
			ZeroMemory(&_desc, sizeof(_desc));

			_desc.Format = ToDXGIFormatSRV(format);

			switch (m_TextureDesc._textureType)
			{
				case TextureType::Texture1D:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
					_desc.Texture1D.MostDetailedMip = baseMipLevel;
					_desc.Texture1D.MipLevels = numMipLevels;
					break;

				case TextureType::Texture2D:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					_desc.Texture2D.MostDetailedMip = baseMipLevel;
					_desc.Texture2D.MipLevels = numMipLevels;
					break;

				case TextureType::Texture3D:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
					_desc.Texture3D.MostDetailedMip = baseMipLevel;
					_desc.Texture3D.MipLevels = numMipLevels;
					break;

				case TextureType::TextureCube:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					_desc.TextureCube.MostDetailedMip = baseMipLevel;
					_desc.TextureCube.MipLevels = numMipLevels;
					break;

				case TextureType::Texture1DArray:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
					_desc.Texture1DArray.MostDetailedMip = baseMipLevel;
					_desc.Texture1DArray.MipLevels = numMipLevels;
					_desc.Texture1DArray.FirstArraySlice = baseArrayLayer;
					_desc.Texture1DArray.ArraySize = numArrayLayers;
					break;

				case TextureType::Texture2DArray:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					_desc.Texture2DArray.MostDetailedMip = baseMipLevel;
					_desc.Texture2DArray.MipLevels = numMipLevels;
					_desc.Texture2DArray.FirstArraySlice = baseArrayLayer;
					_desc.Texture2DArray.ArraySize = numArrayLayers;
					break;

				case TextureType::TextureCubeArray:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					_desc.TextureCubeArray.MostDetailedMip = baseMipLevel;
					_desc.TextureCubeArray.MipLevels = numMipLevels;
					_desc.TextureCubeArray.First2DArrayFace = baseArrayLayer;
					_desc.TextureCubeArray.NumCubes = numArrayLayers / 6;
					break;

				case TextureType::Texture2DMS:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
					break;

				case TextureType::Texture2DMSArray:
					_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					_desc.Texture2DMSArray.FirstArraySlice = baseArrayLayer;
					_desc.Texture2DMSArray.ArraySize = numArrayLayers;
					break;
			}

			/*_desc.Format = bUseStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
			_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			_desc.Texture2D.MipLevels = 1;*/
			_desc.Format = ToDXGIFormatSRV(_desc.Format);
			HR(device->CreateShaderResourceView(resource, &_desc, srv)
				, "failed to create D3D11 shader-resource-view");
		}

		void DX11Texture::CreateUnorderedAccessView(ID3D11Device* device, uint32 baseMipLevel, uint32 baseArrayLayer, uint32 numArrayLayers)
		{
			CreateSubresourceUAV(
				device,
				m_NativeTexture._resource.Get(),
				m_UnorderedAccessView.GetAddressOf(),
				m_TextureDesc._textureType,
				m_Format,
				baseMipLevel,
				baseArrayLayer, 
				numArrayLayers);
		}

		void DX11Texture::CreateSubresourceUAV(ID3D11Device* device, ID3D11Resource* resource, ID3D11UnorderedAccessView** uav, const TextureType type, const DXGI_FORMAT format, uint32 baseMipLevel, uint32 baseArrayLayer, uint32 numArrayLayers)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC _uavDesc;
			ZeroMemory(&_uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

			_uavDesc.Format = ToDXGIFormatUAV(format);

			switch (type)
			{
				case TextureType::Texture1D:
				{
					_uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
					_uavDesc.Texture1D.MipSlice = baseMipLevel;
					break;
				}
				case TextureType::Texture2D:
				{
					_uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
					_uavDesc.Texture2D.MipSlice = baseMipLevel;
					break;
				}
				case TextureType::Texture3D:
				{
					_uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
					_uavDesc.Texture3D.MipSlice = baseMipLevel;
					_uavDesc.Texture3D.FirstWSlice = baseArrayLayer;
					_uavDesc.Texture3D.WSize = -1;//numArrayLayers; 왜지??
					break;
				}
				case TextureType::Texture1DArray:
				{
					_uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
					_uavDesc.Texture1DArray.MipSlice = baseMipLevel;
					_uavDesc.Texture1DArray.FirstArraySlice = baseArrayLayer;
					_uavDesc.Texture1DArray.ArraySize = numArrayLayers;
					break;
				}
				case TextureType::Texture2DArray:
				case TextureType::TextureCube:
				case TextureType::TextureCubeArray:
				{
					_uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
					_uavDesc.Texture2DArray.MipSlice = baseMipLevel;
					_uavDesc.Texture2DArray.FirstArraySlice = baseArrayLayer;
					_uavDesc.Texture2DArray.ArraySize = numArrayLayers;
					break;
				}
				case TextureType::Texture2DMS:
				case TextureType::Texture2DMSArray:
				{
					break;
				}
			}

			HR(device->CreateUnorderedAccessView(resource, &_uavDesc, uav)
				, "failed to create D3D11 unorder-resource-view");
		}

		void DX11Texture::CreateTextureFromFile(ID3D11Device* device, const ImageDesc& srcDesc)
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

			AssertMessageBox(image.GetImageCount() != 0, "D3D11Texture LoadFaile Error");

			bool isCubeMap = image.GetMetadata().IsCubemap();

			if (isCubeMap)
			{
				DirectX::ScratchImage mipChain;

				HR(CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf())
				, "failed to load D3D11 cube texture");

			}
			else
			{
				HR(CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), m_NativeTexture._resource.GetAddressOf())
				, "failed to load D3D11 2D texture");

			}

			/*ScratchImage _compressImage;
			DirectX::Compress(device
				, *image.GetImages()
				, DXGI_FORMAT_BC1_TYPELESS
				, TEX_COMPRESS_DEFAULT
				, 0.0
				, _compressImage);*/

			D3D11_TEXTURE2D_DESC textureDesc;
			m_NativeTexture._tex2D->GetDesc(&textureDesc);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			srvDesc.Format = textureDesc.Format;
			srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
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

			HR(device->CreateShaderResourceView(m_NativeTexture._resource.Get(), &srvDesc, m_ShaderResourceView.ReleaseAndGetAddressOf())
			, "failed to load shader-resource-view");

			m_TextureDesc._format = UnmapFormat(textureDesc.Format);
			m_TextureDesc._extend = { textureDesc.Width, textureDesc.Height, 0 };
			m_TextureDesc._mipLevels = textureDesc.MipLevels;
			m_TextureDesc._bindFlags = textureDesc.BindFlags;
		}

		void DX11Texture::SetName(const char* name)
		{
			DX11SetObjectName(GetNativeTexture()._resource.Get(), name);

			if (m_ShaderResourceView)
				DX11SetObjectNameSubscript(m_ShaderResourceView.Get(), name, ".SRV");
			if (m_UnorderedAccessView)
				DX11SetObjectNameSubscript(m_UnorderedAccessView.Get(), name, ".UAV");
		}

		FileFormat DX11Texture::CheckFileFormat(const tstring& path)
		{
			if (static_cast<uint32>(path.length()) > 1)
			{
				size_t extensionStartPoint = path.find_last_of('.') + (size_t)1;

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