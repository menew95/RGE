#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\MonoBehaviour.h"

namespace GameEngine
{
	namespace Core
	{
		MonoBehaviour::MonoBehaviour(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("MonoBehaviour")*/)
			: Component(gameObject, componentName)
		{

		}

		MonoBehaviour::~MonoBehaviour()
		{

		}

	}
}