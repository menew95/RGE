#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Scene.h"

namespace GameEngine
{
	namespace Core
	{

		Scene::Scene(const tstring& _typeName /*= TEXT("Scene")*/)
			: Object(_typeName)
		{

		}

		void Scene::Resize(size_t size)
		{
			m_GameObjects.resize(size);
		}

		void Scene::AddGameObject(std::shared_ptr<GameObject>& gameObject)
		{
			m_GameObjects.emplace_back(gameObject);
		}

	}
}