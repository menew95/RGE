#pragma once

#include "GraphicsModule/Utility/Export.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC RenderSystemObject
	{
	public:
		virtual void SetName(const char* name) {}
	};
}