#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		Component::Component(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("Component")*/)
			: Object(componentName)
			, m_pGameObject(gameObject)
			, m_bEnable(true)
		{

		}

		Component::~Component()
		{

		}

		tstring& Component::GetTag()
		{
			return m_pGameObject->GetTag();
		}

		void Component::SetTag(const tstring& value)
		{
			m_pGameObject->SetTag(value);
		}

		void Component::SetEnable(bool value)
		{
			if (/*m_pGameObject->GetActiveInHierarchy() &&*/ m_bEnable != value)
			{
				m_bEnable = value;

				m_bEnable ? OnEnable() : OnDisable();
			}
		}

		std::shared_ptr<Transform>& Component::GetTransform()
		{
			return m_pGameObject->GetTransform();
		}

		std::vector<std::shared_ptr<Component>>& Component::GetComponents()
		{
			return m_pGameObject->GetComponents();
		}

	}

}