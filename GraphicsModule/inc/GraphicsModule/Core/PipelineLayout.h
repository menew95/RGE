#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC PipelineLayout
	{
	public:
		virtual uint32 GetNumBindings() const abstract;
	};
}