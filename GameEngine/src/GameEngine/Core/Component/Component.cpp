#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"

namespace GameEngine
{
	namespace Core
	{
		Component::Component(class GameObject* gameObject, const tstring& componentName /*= TEXT("Component")*/)
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

		Transform& Component::GetTransform()
		{
			return m_pGameObject->GetTransform();
		}

		std::vector<Component*>& Component::GetComponents()
		{
			return m_pGameObject->GetComponents();
		}

	}

}