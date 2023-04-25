#pragma once

#include "Common.h"

#include "GraphicsEngine/Resource/LightBuffer.h"

/*
* Light, 쉐도우 관련 버퍼 관리 담당
* 
* 라이트 버퍼를 생성 및 라이트 관련 정보를 텍스처에 업데이트
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
	class RenderObject;

	struct Lighting;

	class Light
	{
	public:
		Light(RenderSystem* renderSystem, CommandBuffer* commandBuffer, ResourceManager* resourceManager);
		~Light();

		LightBuffer* AddLight();

		//void GetLightingData(struct Lighting& perLightFrame);
		Lighting* GetLightingData();

		void UpdateLightTexture();

		void ExcutePass();

		void RegistStaticRenderObject(RenderObject* renderObject);
		void RegistSkinnedRenderObject(RenderObject* renderObject);

	private:
		void Init();

		void Culling(DirectionLight& pointLight);
		void Culling(PointLight& pointLight);
		void Culling(SpotLight& pointLight);

		void CreateRenderTarget();

		RenderSystem* m_RenderSystem;
		CommandBuffer* m_CommandBuffer;
		ResourceManager* m_ResourceManager;

		MaterialBuffer* m_Deferred_Light_Material;
		MeshBuffer* m_Screen_Mesh;

		Texture* m_LightTexture;

		Texture* m_PointLightShadowTexture;
		Texture* m_SpotLightShadowTexture;

		std::vector<RenderObject*> m_StaticRenderObjectList;
		std::vector<RenderObject*> m_SkinnedRenderObjectList;

		std::shared_ptr<Graphics::RenderPass> m_Deferred_Light_Pass;

		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_CascadedShadow_Skinned_Pass;

		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_PointShadow_Skinned_Pass;

		std::shared_ptr<Graphics::RenderPass> m_SpotShadow_Pass;
		std::shared_ptr<Graphics::RenderPass> m_SpotShadow_Skinned_Pass;

		std::vector<std::shared_ptr<LightBuffer>> m_LightBuffers;

		std::vector<Graphics::RenderTarget*> m_SpotShadowRenderTarget;
		std::vector<Graphics::RenderTarget*> m_PointShadowRenderTarget;

		Lighting m_Lighting;
		
	};
}