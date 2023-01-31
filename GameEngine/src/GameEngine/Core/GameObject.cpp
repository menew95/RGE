#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Transform.h"

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

	}
}