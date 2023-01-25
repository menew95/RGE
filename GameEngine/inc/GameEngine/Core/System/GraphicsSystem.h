#pragma once

#include "Common.h"

namespace Graphics
{
	class RenderPass;
	class GraphicsEngine;

	class MeshBuffer;
	class MaterialBuffer;
	class CameraBuffer;
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

			Graphics::Texture* LoadTexture(uuid _uuid);

			void DeleteMeshBuffer(Graphics::MeshBuffer*);
			void DeleteMaterialBuffer(Graphics::MaterialBuffer*);
			void DeleteTextureBuffer(Graphics::Texture*);

		protected:
			void LoadGraphicsEngineDll();
			void FreeGraphicsEngineDll();

			std::vector<std::shared_ptr<Graphics::RenderPass>> m_RenderPassList;

			std::unique_ptr<Graphics::GraphicsEngine> m_GraphicsEngine;
		};
	}
}