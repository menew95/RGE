#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Transform.h"

namespace GameEngine
{
	namespace Core
	{

		GameObject::GameObject(const tstring& gameObjectName /*= TEXT("GameObject")*/)
			: Object(TEXT("GameObject"))
		{

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
					_transIter->GetGameObject()->SetActiveInHierarchy(value);
				}
			}
		}

	}
}