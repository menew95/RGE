/**

    @file      GraphicsSystem.h
    @brief     �׷��Ƚ� ������ �����ϴ� �̱��� ��ü
    @details   
    @author    LWT
    @date      7.06.2023

**/
#pragma once

#pragma once

#include "Common.h"

#include "GraphicsEngine/GraphicsEngineFlags.h"

namespace Graphics
{
	class RenderPass;
	class GraphicsEngine;
	class RenderObject;
	class MeshBuffer;
	class MaterialBuffer;
	class CameraBuffer;
	class LightBuffer;
	class Texture;
	class Resource;
}

namespace GameEngine
{

	namespace Core
	{
		class Mesh;
		class Material;

		class GAME_ENGINE_API GraphicsSystem
		{
			DECLARE_SINGLETON_CLASS(GraphicsSystem)
		public:

			void Render();

			void Initialize();

			void CreateMeshBuffer(std::shared_ptr<Mesh>& mesh);
			void CreateMaterialBuffer(std::shared_ptr<Material>& material);

			Graphics::CameraBuffer* CreateCameraBuffer();
			Graphics::LightBuffer* CreateLightBuffer();

			Graphics::Texture* LoadTexture(uuid _uuid);
			Graphics::Resource* GetResource(uuid _uuid, uint32 type);

			void ReleaseMeshBuffer(Graphics::MeshBuffer* meshBuffer);
			void ReleaseMaterialBuffer(Graphics::MaterialBuffer* materialBuffer);
			void ReleaseLightBuffer(Graphics::LightBuffer* lightBuffer);

		   /**
			   @brief �׷��Ƚ� �������� ���� ���� ��ü ����
			   @param ���� ������Ʈ�� ���� ��ü ������
		   **/
			Graphics::RenderObject* CreateRenderObject();

		   /**
			   @brief �׷��Ƚ� ������ ����� ���� ������Ʈ ����
			   @param renderObject - ������ ���� ������Ʈ
		   **/
			void RemoveRenderObject(Graphics::RenderObject* renderObject);

			Graphics::RenderingSetting& GetRenderingSetting() { return m_RenderingSetting; }
			void SetRenderingSetting(Graphics::RenderingSetting setting);
		protected:
			void LoadGraphicsEngineDll();
			void FreeGraphicsEngineDll();
			
			bool m_IsSettingChange = false;

			Graphics::RenderingSetting m_RenderingSetting;

			std::vector<std::pair<tstring, Graphics::RenderPass*>> m_RenderPassList;

			std::unique_ptr<Graphics::GraphicsEngine> m_GraphicsEngine;
		};
	}
}