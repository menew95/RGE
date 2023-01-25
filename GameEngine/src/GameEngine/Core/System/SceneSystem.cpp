#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\SceneSystem.h"
#include "GameEngine\Core\System\ComponentSystem.h"
#include "GameEngine\Core\Scene.h"
#include "GameEngine\Core\GameObject.h"


#include "Test/TestScene.h"

namespace GameEngine
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(SceneSystem, {}, {})

		bool SceneSystem::LoadScene(uint32 index)
		{
			try
			{
				if (index >= (uint32)m_Scenes.size())
					throw index;

				auto* _comSys = ComponentSystem::GetInstance();

				std::vector<std::shared_ptr<GameObject>>& _gameObjects = m_Scenes[index]->GetAllGameObject();

				for (size_t _objIndex = 0; _objIndex < _gameObjects.size(); _objIndex++)
				{
					auto& _coms = _gameObjects[_objIndex]->GetComponents();

					for (size_t _comIndex = 0; _comIndex < _coms.size(); _comIndex++)
					{
						_comSys->ReserveAddComponent(_coms[_comIndex]);
					}
				}

				return true;
			}
			catch (uint32 errorIndex)
			{
				// Todo : errorIndex 로그 연결

				return errorIndex;
			}
		}

		void SceneSystem::Initialize()
		{
			std::shared_ptr<Scene> _testScene = std::make_shared<TestScene>();

			m_Scenes.push_back(_testScene);
		}

	}
}