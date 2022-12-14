#pragma once

#include "GraphicsModule/Core/Resource.h"
#include "GraphicsModule/Core/SamplerFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC Sampler : public Resource
	{
	public:
		Sampler() = default;
		virtual ~Sampler() = default;

		virtual void SetName(const char* name) abstract;

		ResourceType GetResourceType() const override final;
	};
}