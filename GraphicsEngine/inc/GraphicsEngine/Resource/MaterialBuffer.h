#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsEngine/RenderObject.h"

namespace Graphics
{
	class GRAPHICSENGINE_DLL_DECLSPEC MaterialBuffer : public ResourceBuffer
	{
	public:
		MaterialBuffer(Graphics::RenderSystem* renderSystem);
		~MaterialBuffer() override;

		inline const std::vector<UpdateResourceData>& GetUpdateReosurceData() const
		{
			return m_UpdateResources;
		}

	private:

		struct Standard
		{
			Math::Color _albedo = Math::Color::White;
			Math::Color _emissvie = Math::Color::White;
			float _roughness = 0.0f;
			float _metallic = 0.0f;
			Math::Vector2 _tile = Math::Vector2::One;
		};
		


		std::vector<UpdateResourceData> m_UpdateResources;
	};
}