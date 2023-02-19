#include "GraphicsPCH.h"
#include "GraphicsModule/Core/TextureFlags.h"

namespace Graphics
{
	
	GRAPHICS_DLL_DECLSPEC bool IsCubeTexture(const TextureType& type)
	{
		return (type == TextureType::TextureCube || type == TextureType::TextureCubeArray);
	}

	GRAPHICS_DLL_DECLSPEC bool IsMultiSampleTexture(const TextureType& type)
	{
		return (type >= TextureType::Texture2DMS);
	}

	GRAPHICS_DLL_DECLSPEC bool IsMipMappedTexture(const TextureDesc& desc)
	{
		return (!IsMultiSampleTexture(desc._textureType) && (desc._mipLevels == 0 || desc._mipLevels > 1));
	}

	GRAPHICS_DLL_DECLSPEC uint32 NumMipLevels(std::uint32_t width, std::uint32_t height, std::uint32_t depth)
	{
		const auto maxSize = std::max({ width, height, depth });
		const auto log2Size = static_cast<uint32>(std::log2(maxSize));
		return (1u + log2Size);
	}
}