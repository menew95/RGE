#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"
#include "GameEngine\Core\System\ComponentSystem.h"

namespace GameEngine
{
	namespace Core
	{

		void ComponentSystem::RegistComponent(uint32 order, const tstring& componentName)
		{
			auto _iter = std::ranges::find_if(m_ComponentsList.begin()
				, m_ComponentsList.end()
				, [&componentName](Components& components)
				{
					return components == componentName;
				}
			);

			if (_iter == m_ComponentsList.end())
			{
				m_ComponentsList.push_back(Components(order, componentName));
			}
			else
			{
				assert(false);
			}
		}

		void ComponentSystem::ReserveAddComponent(std::shared_ptr<Component>& component)
		{
			auto _iter = std::ranges::find_if(m_ComponentsList.begin()
				, m_ComponentsList.end()
				, [&component](Components& components)
				{
					return components == component->GetTypeName();
				}
			);

			if (_iter != m_ComponentsList.end())
			{
				(*_iter).AddComponent(component);
			}
			else
			{
				assert(false);
			}
		}

		void ComponentSystem::ReserveDeleteComponent(std::shared_ptr<Component>& component)
		{
			auto _iter = std::ranges::find_if(m_ComponentsList.begin()
				, m_ComponentsList.end()
				, [&component](Components& components)
				{
					return components == component->GetTypeName();
				}
			);

			if (_iter != m_ComponentsList.end())
			{
				(*_iter).AddComponent(component);
			}
			else
			{
				assert(false);
			}
		}

		void ComponentSystem::UpdateComponent()
		{
			for (auto _components : m_ComponentsList)
			{
				_components.StartComponents();
				_components.UpdateComponents();
				_components.DestroyComponents();
			}
		}

		/*void ComponentSystem::AddComponents()
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

		}*/
	}
}
