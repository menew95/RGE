#include "GraphicsPCH.h"
#include "GraphicsModule/Core/Texture.h"

namespace Graphics
{
	ResourceType Texture::GetResourceType() const
	{
		return ResourceType::Texture;
	}

	Texture::Texture(const TextureType& type, uint32 bindFlags)
		: m_TextureType(type)
		, m_BindFlags(bindFlags)
	{

	}
}