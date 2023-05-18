#pragma once

#include "GraphicsModule/Core/ResourceViewFlags.h"
#include "GraphicsModule/Core/Resource.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC ResourceView : public Resource 
	{
	public:
		virtual ~ResourceView() = default;

		virtual void BindForGraphicsPipeline(
			ID3D11DeviceContext* context,
			uint32 slot,
			uint32 count,
			long bindFlags,
			long stageFlags) abstract;
		virtual void BindForComputePipeline(
			ID3D11DeviceContext* context,
			uint32 slot,
			uint32 count,
			long bindFlags,
			long stageFlags) abstract;

		ResourceType GetResourceType() const override;
	};
}