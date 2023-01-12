#pragma once

#include "Common.h"

namespace Graphics
{
	class RenderPass;
	class GraphicsEngine;
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

		protected:
			void LoadGraphicsEngineDll();
			void FreeGraphicsEngineDll();

			std::vector<std::shared_ptr<Graphics::RenderPass>> m_RenderPassList;

			std::unique_ptr<Graphics::GraphicsEngine> m_GraphicsEngine;
		};
	}
}