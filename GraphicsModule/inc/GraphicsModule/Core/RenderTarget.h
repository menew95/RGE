#pragma once

#include "GraphicsModule/Core/RenderSystemObject.h"

#include "GraphicsModule/Core/RenderTargetFlags.h"

namespace Graphics
{

	class GRAPHICS_DLL_DECLSPEC RenderTarget : public RenderSystemObject
	{
	public:
		RenderTarget() = default;
		virtual ~RenderTarget() = default;

		enum class Type
		{
			SwapChain,
			RT
		};

		inline Type GetType() { return m_Type; }

	protected:
		Type m_Type = Type::RT;
	};
}