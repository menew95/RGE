#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Component.h"

#include "GameEngine\Core\System\ComponentSystem.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::Component>("Component")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_Typename",  &GameEngine::Core::Component::GetType, &GameEngine::Core::Component::SetType)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::WSTRING)
	)
	.property("m_UUID",  &GameEngine::Core::Component::GetUUID, &GameEngine::Core::Component::SetUUID)
	.property("m_Enable", &GameEngine::Core::Component::GetEnable, &GameEngine::Core::Component::SetEnable)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::BOOL)
	)
	.property("m_bIsActiveAndEnabled", &GameEngine::Core::Component::m_bIsActiveAndEnabled)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::BOOL)
	)
	.method("GetGameObjectName", &GameEngine::Core::Component::GetGameObjectName)
	.method("GetGameObjectUUID", &GameEngine::Core::Component::GetGameObjectUUID)
	.method("GetUUID", &GameEngine::Core::Component::GetUUID);
}

static uint32 _order = 0;

namespace GameEngine
{
	namespace Core
	{
		Component::Component(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("Component")*/)
			: Object(componentName)
			, m_pGameObject(gameObject)
			, m_bEnable(true)
		{
			//ComponentSystem::GetInstance()->RegistComponent(_order++, componentName);
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

		tstring Component::GetGameObjectName()
		{
			return m_pGameObject->GetName();
		}

		uuid Component::GetGameObjectUUID()
		{
			return m_pGameObject->GetUUID();
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