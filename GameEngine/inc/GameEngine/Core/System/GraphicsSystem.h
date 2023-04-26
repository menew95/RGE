#pragma once

#include "Common.h"

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

			void DeleteMeshBuffer(Graphics::MeshBuffer*);
			void DeleteMaterialBuffer(Graphics::MaterialBuffer*);
			void DeleteTextureBuffer(Graphics::Texture*);
			void DeleteLightBuffer(Graphics::LightBuffer*);

			void RegistRenderObject(Graphics::RenderObject* renderObject);
			void DeleteRenderObject(Graphics::RenderObject* renderObject);

			//void RegistRenderObject(const tstring& passName, Graphics::RenderObject& renderObject);
			void RegistRenderObject(uint32 passIdx, Graphics::RenderObject& renderObject);
			void RegistShadowObject(uint32 type, Graphics::RenderObject& renderObject);

		protected:
			void LoadGraphicsEngineDll();
			void FreeGraphicsEngineDll();

			//std::vector<std::pair<tstring, std::shared_ptr<Graphics::RenderPass>>> m_RenderPassList;

			std::vector<std::pair<tstring, Graphics::RenderPass*>> m_RenderPassList;

			std::unique_ptr<Graphics::GraphicsEngine> m_GraphicsEngine;
		};
	}
}