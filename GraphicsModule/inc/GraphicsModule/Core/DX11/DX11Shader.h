#pragma once

#include "GraphicsModule/Core/Shader.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11Shader : public Shader
		{
		public:
			DX11Shader(const ShaderDesc& desc);

		};
	}
}