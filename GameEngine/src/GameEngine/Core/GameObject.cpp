#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Transform.h"

//using namespace rttr;
//RTTR_REGISTRATION
//{
//	registration::class_<GameEngine::Core::GameObject>("GameObject")
//		.constructor<const tstring&>()
//		.property("m_Layer", GameEngine::Core::GameObject::GetLayer, GameEngine::Core::GameObject::SetLayer);
//		//.property("m_Components", GameEngine::Core::GameObject::m_Components, registration::protected_access);
//}

namespace GameEngine
{
	namespace Core
	{

		GameObject::GameObject(const tstring& gameObjectName /*= TEXT("GameObject")*/)
			: Object(TEXT("GameObject"))
			, m_Layer(0)
			, m_bActiveInHierarchy(true)
			, m_bIsActiveSelf(true)
			, m_bIsStatic(false)
		{
		}

		void GameObject::AddTransform()
		{
			m_pTransform = AddComponent<Transform>();
		}

		std::shared_ptr<GameObject> GameObject::Instantiate()
		{
			std::shared_ptr<GameObject> _newGamObject = std::make_shared<GameObject>();

			_newGamObject->m_pTransform = _newGamObject->AddComponent<Transform>();

			return _newGamObject;
		}

		std::shared_ptr<GameEngine::Core::GameObject> GameObject::Create()
		{
			std::shared_ptr<GameObject> _newGamObject = std::make_shared<GameObject>();

			return _newGamObject;
		}

		void GameObject::SetActiveInHierarchy(bool value)
		{
			if (m_bActiveInHierarchy != value)
			{
				m_bActiveInHierarchy = value;

				for (auto& _transIter : m_pTransform->GetChilds())
				{
					_transIter->GetGameObject()->SetActiveInHierarchy(value);
				}
			}
		}

		void GameObject::SetActiveSelf(bool value)
		{
			m_bIsActiveSelf = value;

			if (m_bActiveInHierarchy)
			{
				m_bActiveInHierarchy = value;

				for (auto& _transIter : m_pTransform->GetChilds())
				{
					auto& _obj = _transIter->GetGameObject();

					if (_obj->GetActiveSelf() == true)
					{
						_obj->SetActiveInHierarchy(value);
					}
				}
			}
		}

		GameObject* GameObject::FindGameObject(const tstring& goName)
		{
			GameObject* _findGameObject = nullptr;

			if (GetName() == goName)
			{
				return this;
			}

			for (const auto& _child : m_pTransform->GetChilds())
			{
				_findGameObject = _child->GetGameObject()->FindGameObject(goName);

				if (_findGameObject != nullptr)
				{
					return _findGameObject;
				}
			}

			return _findGameObject;
		}

		bool GameObject::AddComponent(Component* component)
		{
			m_Components.push_back(std::shared_ptr<Component>(component));

			return true;
		}

	}
}