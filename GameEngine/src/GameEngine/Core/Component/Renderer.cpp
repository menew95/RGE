#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\Renderer.h"
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