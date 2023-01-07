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

}