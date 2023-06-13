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
			, m_RenderObject(nullptr)
			, m_bIsShadowCasting(true)
		{
		}

		Renderer::~Renderer()
		{
			GraphicsSystem::GetInstance()->RemoveRenderObject(m_RenderObject);
		}

		void Renderer::Awake()
		{
			m_RenderObject = GraphicsSystem::GetInstance()->CreateRenderObject();
		}

		void Renderer::OnEnable()
		{
			if (!m_RenderObject) m_RenderObject->m_bIsEnable = true;


		}

		void Renderer::OnDisalbe()
		{
			if (!m_RenderObject) m_RenderObject->m_bIsEnable = false;
		}

		void Renderer::SetShadowCasting(bool value)
		{
			m_bIsShadowCasting = value;

			if(!m_RenderObject) m_RenderObject->m_bIsCastShadow = value;
		}
	}
}