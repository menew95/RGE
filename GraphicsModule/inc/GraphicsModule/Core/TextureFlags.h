#pragma once

#include "Common.h"
#include "Math/Math.h"
#include "GraphicsModule/Core/Format.h"
#include "GraphicsModule/Core/ResourceFlags.h"
#include "GraphicsModule/Core/CommandBufferFlags.h"

namespace Graphics
{
	enum class TextureType
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DArray,
		Texture2DArray,
		TextureCubeArray,
		Texture2DMS,
		Texture2DMSArray,
	};

	//enum class TextureSwizzle
	//{
	//	Zero,
	//	One,
	//	Red,
	//	Green,
	//	Blue,
	//	Alpha
	//};

	struct TexturSubresource
	{
		uint32 _baseArrayLayer = 0;
		uint32 _numArrayLayers = 0;
		uint32 _baseMipLevel = 0;
		uint32 _numMipLevels = 0;
	};

	enum class FileFormat
	{
		DDS = 0,
		TGA = 1,
		HDR = 2,
		WIC = 3,
	};

	struct ImageDesc
	{
		tstring _filePath;
		void* _data;
	};

	struct TextureSubresource
	{
		TextureSubresource() = default;
		TextureSubresource(const TextureSubresource&) = default;

		inline TextureSubresource(uint32 baseArrayLayer, uint32 baseMipLevel) :
			baseArrayLayer{ baseArrayLayer },
			baseMipLevel{ baseMipLevel }
		{
		}

		inline TextureSubresource(uint32 baseArrayLayer, uint32 numArrayLayers, uint32 baseMipLevel, uint32 numMipLevels) :
			baseArrayLayer{ baseArrayLayer },
			numArrayLayers{ numArrayLayers },
			baseMipLevel{ baseMipLevel },
			numMipLevels{ numMipLevels }
		{
		}

		uint32   baseArrayLayer = 0;
		uint32   numArrayLayers = 1;
		uint32   baseMipLevel = 0;
		uint32   numMipLevels = 1;
	};

	struct TextureRegion
	{
		TextureRegion() = default;
		TextureRegion(const TextureRegion&) = default;

		inline TextureRegion(const Offset3D& offset, const Extent3D& extent) :
			offset{ offset },
			extent{ extent }
		{
		}

		inline TextureRegion(const TextureSubresource& subresource, const Offset3D& offset, const Extent3D& extent) :
			subresource{ subresource },
			offset{ offset },
			extent{ extent }
		{
		}
		TextureSubresource  subresource;

		Offset3D            offset;

		Extent3D            extent;
	};

	struct TextureDesc
	{
		TextureType _textureType = TextureType::Texture2D;
		uint32 _bindFlags = (BindFlags::ShaderResource | BindFlags::RenderTarget);
		uint32 _miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
		Format _format = Format::R8G8B8A8_UNORM;
		Extent3D _extend = { 1, 1, 1 };
		uint32 _arrayLayers = 1;
		uint32 _mipLevels = 0;
		uint32 _samples = 1;
		//ClearValue _clearValue;
	};

	GRAPHICS_DLL_DECLSPEC bool IsCubeTexture(const TextureType& type);
	GRAPHICS_DLL_DECLSPEC bool IsMultiSampleTexture(const TextureType& desc);
	GRAPHICS_DLL_DECLSPEC bool IsMipMappedTexture(const TextureDesc& desc);
	GRAPHICS_DLL_DECLSPEC uint32 NumMipLevels(std::uint32_t width, std::uint32_t height, std::uint32_t depth);
}