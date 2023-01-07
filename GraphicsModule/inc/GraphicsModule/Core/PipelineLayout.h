#pragma once

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class PipelineLayout
	{
	public:
		virtual uint32 GetNumBindings() const abstract;
	};
}