#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"
#include "GameEngine\Core\System\ComponentSystem.h"

namespace GameEngine
{
	namespace Core
	{
		void ComponentSystem::ReserveAddComponent(std::shared_ptr<Component>& component)
		{
			m_WaitForAddComponents.emplace_back(component);
		}

		void ComponentSystem::ReserveDeleteComponent(std::shared_ptr<Component>& component)
		{
			auto _find = std::ranges::find(m_WaitForDeleteComponents, component);

			if (_find != m_WaitForDeleteComponents.end())
			{
				m_WaitForDeleteComponents.emplace_back(component);
			}
		}

		void ComponentSystem::UpdateComponent()
		{
			StartComponent();

			for (auto& _componentIter : m_WaitForUpdateComponents)
			{
				if(_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
				{
					_componentIter->Update();
				}
			}

			for (auto& _componentIter : m_WaitForUpdateComponents)
			{
				if (_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
				{
					_componentIter->LateUpdate();
				}
			}
		}

		void ComponentSystem::AddComponents()
		{
			for (auto iter = m_WaitForAddComponents.begin(); iter != m_WaitForAddComponents.end(); iter++)
			{
				(*iter)->Awake();

				if ((*iter)->GetEnable() && (*iter)->GetGameObject()->GetActiveInHierarchy())
				{
					m_WaitForStartComponents.emplace_back(*iter);
					m_WaitForAddComponents.erase(iter);
				}
			}
		}

		void ComponentSystem::StartComponent()
		{
			for (auto& component : m_WaitForStartComponents)
			{
				if (component->GetEnable() && component->GetGameObject()->GetActiveInHierarchy())
				{
					component->Start();

					m_WaitForUpdateComponents.emplace_back(component);
				}
			}

			m_WaitForStartComponents.clear();
		}

		void ComponentSystem::DeleteComponent()
		{

		}
	}
}
