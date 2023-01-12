#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC PipelineLayout
	{
	public:
		virtual void SetResources(std::vector<Resource*>& resources) abstract;
		virtual void SetResource(uint32 index, Resource* resource) abstract;
		virtual uint32 GetNumBindings() const abstract;
	};
}