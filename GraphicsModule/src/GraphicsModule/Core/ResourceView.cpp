#include "GraphicsPCH.h"
#include "GraphicsModule/Core/ResourceView.h"

namespace Graphics
{
	ResourceType ResourceView::GetResourceType() const
	{
		return ResourceType::ResourceView;
	}
}