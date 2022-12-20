#pragma once
#include "GameEngine\Core\Component\Component.h"

#include "Math\Math.h"

namespace GameEngine
{
	namespace Core
	{
		class GAME_ENGINE_API Renderer
			: public Component
		{
		public:
			Renderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("Renderer"));
			virtual ~Renderer();
		};
	}
}

