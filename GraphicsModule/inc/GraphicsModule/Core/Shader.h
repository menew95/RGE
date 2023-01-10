#pragma once

#include "GraphicsModule/Core/Resource.h"
#include "GraphicsModule/Core/ShaderFlags.h"

namespace Graphics
{
	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void SetName(const char* name) abstract;
	};
}