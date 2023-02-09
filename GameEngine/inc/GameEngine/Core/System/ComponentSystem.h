#pragma once

#include "Common.h"
#include "GameEngine\Core\System\Components.h"

namespace GameEngine
{
	namespace Core
	{
		class Component;

		class GAME_ENGINE_API ComponentSystem
		{
			DECLARE_SINGLETON_CLASS(ComponentSystem)

		public:
			void RegistComponent(uint32 order, const tstring& componentName, bool render = false);

			void ReserveAddComponent(std::shared_ptr<Component>& component);

			void ReserveDeleteComponent(std::shared_ptr<Component>& component);

			void UpdateComponent();

			void RenderComponent();

			void ClearComponentsList();

		private:
			//void AwakeComponent();
			/*void AddComponents();
			void StartComponent();
			void DeleteComponent();*/

			std::vector<Components> m_ComponentsList;

			/*std::vector<std::shared_ptr<Component>> m_WaitForAddComponents;
			std::vector<std::shared_ptr<Component>> m_WaitForDeleteComponents;

			std::vector<std::shared_ptr<Component>> m_WaitForStartComponents;
			std::vector<std::shared_ptr<Component>> m_WaitForUpdateComponents;*/
		};
	}
}