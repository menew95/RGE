#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object\Object.h"

namespace GameEngine
{
	namespace Core
	{
		class Component
			: public Object
		{
		public:
			Component(class GameObject* gameObject, const tstring& componentName = TEXT("Component"));
			virtual ~Component();
			
			tstring& GetTag();
			void SetTag(const tstring& value);
			
			bool GetEnable() { return m_bEnable; }
			void SetEnable(bool value) { m_bEnable = value; }

			class Transform& GetTransform();

			std::vector<Component*>& GetComponents();

			template<typename T>
			T* AddComponent();

			template<typename T>
			T* GetComponent();

			template<typename T>
			T* GetComponentInChildren();

			template<typename T>
			T* GetComponentInParent();

			template<typename T>
			std::vector<T*> GetComponentsInChildren();

			template<typename T>
			std::vector<T*> GetComponentsInParent();

		protected:
			class GameObject* m_pGameObject;

			bool m_bEnable;
		};
	}
}

#include "GameEngine\Core\Component\Component.inl"

#endif