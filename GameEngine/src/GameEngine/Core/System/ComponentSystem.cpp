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
				int i = 0;
				RegistComponent(i++,TEXT("Transform"));
				RegistComponent(i++,TEXT("Camera"));
				RegistComponent(i++,TEXT("MeshFilter"));
				RegistComponent(i++,TEXT("MeshRenderer"), true);
				RegistComponent(i++,TEXT("SkinnedMeshRenderer"), true);
				RegistComponent(i++,TEXT("Animation"));
				RegistComponent(i++,TEXT("Light"));
				RegistComponent(i++,TEXT("CameraController"));
				RegistComponent(i++,TEXT("CubeMove"));
			}, {})

		void ComponentSystem::RegistComponent(uint32 order, const tstring& componentName, bool render)
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

				if (render)
				{
					m_ComponentsList.back().m_IsRender = render;
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
