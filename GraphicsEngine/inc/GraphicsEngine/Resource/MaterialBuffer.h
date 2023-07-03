#pragma once

#include "GraphicsEngine/Resource/ResourceBuffer.h"

#include "GraphicsEngine/RenderObject.h"

#include "GraphicsModule/Core/PipelineLayoutFlags.h"

namespace Graphics
{
	class CommandBuffer;

	enum class TextureBindFlag
	{
		NONE = 0,
		ALBEDO = 1 << 0,
		NORMAL = 1 << 1,
		MRA = 1 << 2,

		ALBEDO_NORMAL = ALBEDO | NORMAL,
		ALBEDO_NORMAL_MRA = ALBEDO_NORMAL | MRA
	};

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

		inline void SetUseInstancing(bool value) { m_bUseInstancing = value; }

		inline bool GetUseInstancing() const { return m_bUseInstancing; }

		void* GetInstanceData() { return &m_PBRMaterialData; }

		inline void SetTextureBindFlags(uint32 flags) { m_TextureBindFlag = flags; }

		inline uint32 GetTextureBindFlags() const { return m_TextureBindFlag; }

		inline void SetPass(RenderPass* pass) { m_RenderPass = pass; }

		inline RenderPass* GetPass() { return m_RenderPass; }

	private:

		bool m_bUseInstancing = true;

		uint32 m_TextureBindFlag;

		struct Standard
		{
			Math::Color _albedo = Math::Color::White;
			Math::Color _emissvie = Math::Color::White;
			float _roughness = 0.0f;
			float _metallic = 0.0f;
			Math::Vector2 _tile = Math::Vector2::One;
		};

		Standard m_PBRMaterialData;

		std::vector<UpdateResourceData> m_UpdateResources;

		struct ResourceBind
		{
			Resource* _resource = nullptr;
			uint32 _slot = 0;
		};

		std::vector<ResourceBind> m_ResourceBindList;

		RenderPass* m_RenderPass;
	};
}