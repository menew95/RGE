#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/RenderPassFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderPass
	{
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;

	private:
	};
}