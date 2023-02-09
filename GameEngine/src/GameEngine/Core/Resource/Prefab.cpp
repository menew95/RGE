#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/GameObject.h"

#include "GameEngine/Core/Resource/Prefab.h"

#include "GameEngine/Core/Component/Transform.h"


namespace GameEngine
{
	namespace Core
	{

		Prefab::Prefab(const tstring& typeName /*= TEXT("Prefab")*/)
			: Resource(typeName)
		{

		}

		Prefab::~Prefab()
		{
			Release();
		}

		void Prefab::SetRootGameObject(std::shared_ptr<GameObject>& rootGameObject)
		{
			m_RootGameObject = rootGameObject;

			AddGameObject(m_RootGameObject);
		}

		void Prefab::RegistGameObject(std::shared_ptr<GameObject>& gameObject)
		{
			m_GameObjectList.push_back(gameObject);

			if (m_RootGameObject == nullptr)
				m_RootGameObject = gameObject;
		}

		std::shared_ptr<GameEngine::Core::GameObject> Prefab::FindGameObject(const tstring& name)
		{
			auto _find = std::find_if(std::begin(m_GameObjectList), std::end(m_GameObjectList),
				[&name](auto& go)
				{
					return go->GetName() == name;
				});

			if (_find != m_GameObjectList.end())
			{
				return *_find;
			}

			return std::shared_ptr<GameEngine::Core::GameObject>(nullptr);
		}

		void Prefab::AddGameObject(std::shared_ptr<GameObject>& gameObject)
		{
			m_GameObjectList.push_back(gameObject);

			for (auto& child : gameObject->GetTransform()->GetChilds())
			{
				AddGameObject(child->GetGameObject());
			}
		}

		void Prefab::Release()
		{

		}

	}
}