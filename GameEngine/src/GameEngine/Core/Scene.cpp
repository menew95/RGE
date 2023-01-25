#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/Scene.h"

#include "GameEngine/Core/Component/Camera.h"
#include "GameEngine/Core/Component/Transform.h"

namespace GameEngine
{
	namespace Core
	{

		Scene::Scene(const tstring& _typeName /*= TEXT("Scene")*/)
			: Object(_typeName)
		{
			std::shared_ptr<GameObject> _camera = GameObject::Instantiate();

			_camera->AddComponent<Camera>();

			AddGameObject(_camera);

			auto _trans = _camera->GetComponent<Transform>();
			
			Math::Vector3 _pos = { 0, 0, -10 };
			_trans->Translate(_pos);
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