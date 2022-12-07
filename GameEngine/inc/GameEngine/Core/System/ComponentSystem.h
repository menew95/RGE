#pragma once

#include "Common.h"

namespace GameEngine
{
	namespace Core
	{
		class Component;

		class ComponentSystem
		{
			DECLARE_SINGLETON_CLASS(ComponentSystem)

		public:
			void ReserveAddComponent(std::shared_ptr<Component>& component);

			void ReserveDeleteComponent(std::shared_ptr<Component>& component);

			void UpdateComponent();

		private:
			//void AwakeComponent();
			void AddComponents();
			void StartComponent();
			void DeleteComponent();


			std::vector<std::shared_ptr<Component>> m_WaitForAddComponents;
			std::vector<std::shared_ptr<Component>> m_WaitForDeleteComponents;

			std::vector<std::shared_ptr<Component>> m_WaitForStartComponents;
			std::vector<std::shared_ptr<Component>> m_WaitForUpdateComponents;
		};
	}
}