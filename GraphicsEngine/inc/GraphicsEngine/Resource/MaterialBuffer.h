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
		std::vector<UpdateResourceData> m_UpdateResources;
	};
}