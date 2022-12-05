#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\GameObject.h"
#include "GameEngine\Core\Component\MonoBehaviour.h"

namespace GameEngine
{
	namespace Core
	{
		MonoBehaviour::MonoBehaviour(class GameObject* gameObject, const tstring& componentName /*= TEXT("MonoBehaviour")*/)
			: Component(gameObject, componentName)
		{

		}

		MonoBehaviour::~MonoBehaviour()
		{

		}

	}
}