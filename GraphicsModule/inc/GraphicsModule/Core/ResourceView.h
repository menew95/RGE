#pragma once

#include "GraphicsModule/Core/ResourceViewFlags.h"
#include "GraphicsModule/Core/Resource.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC ResourceView : public Resource 
	{
	public:
		virtual ~ResourceView() = default;

		ResourceType GetResourceType() const override;
	};
}