#pragma once

#include "GraphicsModule/Utility/Export.h"

#include "GraphicsModule/Core/ShaderFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void SetName(const char* name) abstract;
	};
}