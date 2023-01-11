#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/RenderTargetFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderTarget
	{
	public:
		RenderTarget() = default;
		virtual ~RenderTarget() = default;

	protected:
	};
}