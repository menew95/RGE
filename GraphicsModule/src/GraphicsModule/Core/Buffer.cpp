#include "GraphicsPCH.h"
#include "GraphicsModule/Core/Buffer.h"

namespace Graphics
{
	Buffer::Buffer(uint32 bindFlags)
		: m_BindFlags(bindFlags)
	{

	}

	Graphics::ResourceType Buffer::GetResourceType() const
	{
		return ResourceType::Buffer;
	}

}