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
			
			
			// m_ReserveAddComponents�� ����� ������Ʈ�� Ȱ��ȭ �����ϸ� Start�� ȣ���Ѵ�
			// , �ƴϸ� �ٽ� m_ReserveAddComponents�� Push
			void StartComponents();

			// m_Components�� ��ȸ�ϸ� Ȱ��ȭ�� ������Ʈ���� ������Ʈ �Ѵ�.
			void UpdateComponents();
			
			// m_ReserveDeleteComponents�� ��ȸ�ϸ� �ı��� ������Ʈ���� m_Components���� ���� �Ѵ�.
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