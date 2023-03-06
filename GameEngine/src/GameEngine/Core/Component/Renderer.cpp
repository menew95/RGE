#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Renderer.h"


#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::Renderer>("Renderer");
	//.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>();
}

namespace GameEngine
{
	namespace Core
	{
		Renderer::Renderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("Renderer")*/)
			: Component(gameObject, componentName)
		{

		}

		Renderer::~Renderer()
		{

		}

	}
}