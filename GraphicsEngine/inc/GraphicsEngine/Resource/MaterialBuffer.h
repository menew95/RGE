#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsEngine/RenderObject.h"

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class CommandBuffer;

	class GRAPHICSENGINE_DLL_DECLSPEC MaterialBuffer : public ResourceBuffer
	{
	public:
		MaterialBuffer(Graphics::RenderSystem* renderSystem, uuid uuid);
		~MaterialBuffer() override;

		inline const std::vector<UpdateResourceData>& GetUpdateResourceData() const
		{
			return m_UpdateResources;
		}

		void SetResource(Resource* resource, ResourceType type, uint32 bindSlot, uint32 size = 0);

		void BindResource(CommandBuffer* commandBuffer);
		
		void ChangeResource(Resource* resource, uint32 idx);

		void SetBufferData(void* src, Resource* resource);

	private:

		struct Standard
		{
			Math::Color _albedo = Math::Color::White;
			Math::Color _emissvie = Math::Color::White;
			float _roughness = 0.0f;
			float _metallic = 0.0f;
			Math::Vector2 _tile = Math::Vector2::One;
		};
//		Resource* m_StandardBuffer = nullptr;

		std::vector<UpdateResourceData> m_UpdateResources;

		struct ResourceBind
		{
			Resource* _resource = nullptr;
			uint32 _slot = 0;
		};

		std::vector<ResourceBind> m_ResourceBindList;
	};
}