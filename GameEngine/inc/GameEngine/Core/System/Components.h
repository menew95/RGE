#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class Component;

		class GAME_ENGINE_API Components
		{
		public:
			Components(uint32 order, const tstring& componentName);
			virtual ~Components();

			bool operator==(const tstring& name)
			{
				return m_ComponentName == name;
			}

			void AddComponent(std::shared_ptr<Component>& addComponent);
			void DeleteComponent(std::shared_ptr<Component>& deleteComponent);
			
			
			// m_ReserveAddComponents에 예약된 컴포넌트가 활성화 가능하면 Start를 호출한다
			// , 아니면 다시 m_ReserveAddComponents에 Push
			void StartComponents();

			// m_Components를 순회하며 활성화된 컴포넌트들을 업데이트 한다.
			void UpdateComponents();
			
			// m_ReserveDeleteComponents를 순회하며 파괴할 컴포넌트들을 m_Components에서 삭제 한다.
			void DestroyComponents();

			uint32 GetOrder() { return m_Order; }
			void SetOrder(uint32 order) { m_Order = order; }

		private:
			uint32 m_Order;

			tstring m_ComponentName;

			std::vector<std::shared_ptr<Component>> m_Components;
			std::queue<std::shared_ptr<Component>> m_ReserveAddComponents;
			std::vector<std::shared_ptr<Component>> m_ReserveDeleteComponents;
		};
	}
}