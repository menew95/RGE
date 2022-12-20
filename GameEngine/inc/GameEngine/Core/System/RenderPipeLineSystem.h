#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Graphics
	{
		class RenderPipeLine;
	}
	namespace Core
	{
		class GAME_ENGINE_API RenderPipeLineSystem
		{
			DECLARE_SINGLETON_CLASS(RenderPipeLineSystem)
		public:

			void Render();

		protected:
			std::vector<std::shared_ptr<Graphics::RenderPipeLine>> m_RenderPipeLineList;
		};
	}
}