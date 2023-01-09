#include "GraphicsPCH.h"
#include "GraphicsModule/Core/Sampler.h"

namespace Graphics
{
	ResourceType Sampler::GetResourceType() const
	{
		return ResourceType::Sampler;
	}
}