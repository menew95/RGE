#pragma once

#include "Common.h"

#include "GraphicsEngine/Resource/LightBuffer.h"

/*
* Light, ������ ���� ���� ���� ���
* 
* ����Ʈ ���۸� ���� �� ����Ʈ ���� ������ �ؽ�ó�� ������Ʈ
*/

namespace Graphics
{
	class RenderSystem;
	class CommandBuffer;
	class ResourceManager;
	class RenderPass;
	class MaterialBuffer;
	class MeshBuffer;
	class Texture;

	class Light
	{
	public:
		Light(RenderSystem* renderSystem, ResourceManager* resourceManager);
		~Light();

		LightBuffer* AddLight();

		void GetLightingData(struct Lighting& perLightFrame);

		void UpdateLightTexture();

		void ExcutePass();

	private:
		void Init();

		RenderSystem* m_RenderSystem;
		ResourceManager* m_ResourceManager;

		MaterialBuffer* m_Deferred_Light_Material;
		MeshBuffer* m_Screen_Mesh;

		Texture* m_LightTexture;

		std::shared_ptr<Graphics::RenderPass> m_Deferred_Light_Pass;

		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Skinned_Pass;

		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Skinned_Pass;

		std::vector<std::shared_ptr<LightBuffer>> m_LightBuffers;
	};
}