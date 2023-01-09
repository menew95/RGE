#pragma once

#include "GraphicsModule\Core\Resource.h"
#include "GraphicsModule\Core\TextureFlags.h"

namespace Graphics
{
	class Texture : public Resource
	{
	public:
		virtual ~Texture() = default;

		ResourceType GetResourceType() const override;

	protected:
		Texture(const TextureType& type, uint32 bindFlags);

		TextureType	m_TextureType;
		uint32		m_BindFlags = 0;
	};
}