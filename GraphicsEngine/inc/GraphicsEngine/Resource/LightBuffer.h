#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsEngine/Resource/ConstantBuffer.h"

#include "Math/Math.h"


namespace Graphics
{
	struct Camera;

	class GRAPHICSENGINE_DLL_DECLSPEC LightBuffer : public ResourceBuffer
	{
	public:
		LightBuffer(Graphics::RenderSystem* renderSystem);
		~LightBuffer() override;

		inline PerLight& GetPerLight() { return m_PerLight; }

	private:
		PerLight m_PerLight;
	};
}
