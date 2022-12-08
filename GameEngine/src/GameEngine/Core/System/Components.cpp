#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\System\Components.h"
#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{

		Components::Components(uint32 order, const tstring& componentName)
			: m_Order(order)
			, m_ComponentName(componentName)
		{

		}

		Components::~Components()
		{

		}

		void Components::AddComponent(std::shared_ptr<Component>& addComponent)
		{
			m_ReserveAddComponents.push(addComponent);
			addComponent->Awake();
		}

		void Components::DeleteComponent(std::shared_ptr<Component>& deleteComponent)
		{
			auto _find = std::ranges::find(m_ReserveDeleteComponents, deleteComponent);

			if (_find == m_ReserveDeleteComponents.end())
			{
				m_ReserveDeleteComponents.emplace_back(deleteComponent);
			}
		}

		
		void Components::StartComponents()
		{
			size_t _count = m_ReserveAddComponents.size();

			for (size_t i = 0; i < _count; i++)
			{
				auto _component = m_ReserveAddComponents.front();

				m_ReserveAddComponents.pop();

				if (_component->GetEnable() && _component->GetGameObject()->GetActiveInHierarchy())
				{
					_component->Start();
				}
				else
				{
					m_ReserveAddComponents.push(_component);
				}
			}
		}

		void Components::UpdateComponents()
		{
			for (auto& _componentIter : m_Components)
			{
				if (_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
				{
					_componentIter->Update();
				}
			}

			for (auto& _componentIter : m_Components)
			{
				if (_componentIter->GetEnable() && _componentIter->GetGameObject()->GetActiveInHierarchy())
				{
					_componentIter->LateUpdate();
				}
			}
		}

		void Components::DestroyComponents()
		{
			for (auto& _componentIter : m_ReserveDeleteComponents) 
			{
				auto _find = std::ranges::find(m_Components, _componentIter);

				if (_find != m_Components.end())
				{
					m_Components.erase(_find);
				}
#ifdef _DEBUG
				else
				{
					// m_Components 안에 없다면 에러
					assert(false);
				}
#endif
			}
		}

	}
}