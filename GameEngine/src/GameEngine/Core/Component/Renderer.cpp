#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Renderer.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GraphicsEngine/RenderObject.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::Renderer>("Renderer")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_bIsShadowCasting", &GameEngine::Core::Renderer::m_bIsShadowCasting)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::BOOL)
	);
}

namespace GameEngine
{
	namespace Core
	{
		Renderer::Renderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("Renderer")*/)
			: Component(gameObject, componentName)
		{
			GraphicsSystem::GetInstance()->RegistRenderObject(&m_RenderObject);
		}

		Renderer::~Renderer()
		{
			GraphicsSystem::GetInstance()->DeleteRenderObject(&m_RenderObject);
		}

		void Renderer::OnEnable()
		{
			m_RenderObject.m_bIsEnable = true;
		}

		void Renderer::OnDisalbe()
		{
			m_RenderObject.m_bIsEnable = false;
		}

		void Renderer::SetShadowCasting(bool value)
		{
			m_bIsShadowCasting = value;
			m_RenderObject.m_bIsCastShadow = value;
		}
	}
}