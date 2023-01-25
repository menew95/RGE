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

			// m_Components�� ��ȸ�ϸ� Ȱ��ȭ�� ������Ʈ���� ���� �Ѵ�.
			void RenderComponents();

		};
	}
}