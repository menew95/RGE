#pragma once
#include "GameEngine\Core\Component\Component.h"

#include "Math\Math.h"

namespace GameEngine
{
	namespace Core
	{
		class Renderer
			: public Component
		{
		public:
			Renderer(class GameObject* gameObject, const tstring& componentName = TEXT("Renderer"));
			virtual ~Renderer();
		};
	}
}

