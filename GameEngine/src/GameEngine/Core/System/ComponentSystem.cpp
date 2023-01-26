#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"
#include "GameEngine\Core\System\ComponentSystem.h"

namespace GameEngine
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(ComponentSystem,
			{
				RegistComponent(0,TEXT("Transform"));
				RegistComponent(1,TEXT("Camera"));
				RegistComponent(2,TEXT("MeshFilter"));
				RegistComponent(3,TEXT("MeshRenderer"));
				RegistComponent(4,TEXT("CameraController"));
			}, {})

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

				if (TEXT("MeshRenderer") == componentName)
				{
					m_ComponentsList.back().m_IsRender = true;
				}
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
			for (auto& _components : m_ComponentsList)
			{
				_components.StartComponents();
				_components.UpdateComponents();
				_components.DestroyComponents();
			}
		}

		void ComponentSystem::RenderComponent()
		{
			for (auto& _components : m_ComponentsList)
			{
				_components.RenderComponents();
			}
		}

		void ComponentSystem::ClearComponentsList()
		{
			m_ComponentsList.clear();
		}
	}
}
