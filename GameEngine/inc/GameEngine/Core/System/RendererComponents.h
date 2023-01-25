#pragma once

#include "Common.h"

#include "GameEngine/Core/System/Components.h"

namespace GameEngine
{
	namespace Core
	{
		class Component;

		class GAME_ENGINE_API RendererComponents : public Components
		{
		public:
			RendererComponents(uint32 order, const tstring& componentName);
			virtual ~RendererComponents();

			// m_Components를 순회하며 활성화된 컴포넌트들을 랜더 한다.
			void RenderComponents();

		};
	}
}