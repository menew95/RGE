#pragma once

#include "GraphicsModule/Core/RenderSystemObject.h"

#include "GraphicsModule/Core/ShaderFlags.h"

namespace Graphics
{
	class GRAPHICS_DLL_DECLSPEC Shader : public RenderSystemObject
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;
	};
}