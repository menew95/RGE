#pragma once

#include "Common.h"

namespace Graphics
{
	class RenderPass;
	class GraphicsEngine;

	class MeshBuffer;
	class MaterialBuffer;
}

namespace GameEngine
{

	namespace Core
	{
		class GAME_ENGINE_API GraphicsSystem
		{
			DECLARE_SINGLETON_CLASS(GraphicsSystem)
		public:

			void Render();

			void Initialize();

			void CreateMeshBuffer();
			void CreateMaterialBuffer();

			void DeleteMeshBuffer(Graphics::MeshBuffer*);
			void DeleteMaterialBuffer(Graphics::MaterialBuffer*);

		protected:
			void LoadGraphicsEngineDll();
			void FreeGraphicsEngineDll();

			std::vector<std::shared_ptr<Graphics::RenderPass>> m_RenderPassList;

			std::unique_ptr<Graphics::GraphicsEngine> m_GraphicsEngine;
		};
	}
}