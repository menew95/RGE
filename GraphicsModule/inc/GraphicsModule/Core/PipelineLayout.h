#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class Buffer;
	class Texture;
	class Sampler;

	class GRAPHICS_DLL_DECLSPEC PipelineLayout
	{
	public:
		virtual void SetResources(std::vector<Resource*>& resources) abstract;
		virtual void SetResource(uint32 index, Resource* resource) abstract;
		virtual uint32 GetNumBindings() const abstract;
		virtual Resource* GetResource(uint32 index) abstract;
		virtual Buffer* GetBuffer(uint32 index) abstract;
		virtual Texture* GetTexture(uint32 index) abstract;
		virtual Sampler* GetSampler(uint32 index) abstract;
	};
}