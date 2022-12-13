#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class Scene;

		class SceneSystem
		{
			DECLARE_SINGLETON_CLASS(SceneSystem)
		public:
			uint32 GetSceneCount() { return (uint32)m_Scenes.size(); }

			bool LoadScene(uint32 index);

		private:
			std::vector<std::shared_ptr<Scene>> m_Scenes;
		};
	}
}