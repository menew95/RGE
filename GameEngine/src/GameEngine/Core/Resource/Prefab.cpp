#include "GameEngine/GameEnginePCH.h"
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