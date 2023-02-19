#pragma once

#include "GraphicsModule\Core\Resource.h"
#include "GraphicsModule\Core\TextureFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		ResourceType GetResourceType() const override;

		TextureType GetType() { return m_TextureType; }
	protected:
		Texture(const TextureType& type, uint32 bindFlags);

		TextureType	m_TextureType;
		uint32		m_BindFlags = 0;
	};
}