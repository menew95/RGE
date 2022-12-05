#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object/Object.h"

namespace GameEngine
{
	namespace Core
	{
		class Component;
		class Scene;
		class Transform;

		class GameObject :
			public Object
		{
		public:
			GameObject(const tstring& gameObjectName = TEXT("GameObject"));

			bool CompareTag(const tstring& value) { return value == m_Tag; }

			void SetActiveInHierarchy(bool value) { m_bActiveInHierarchy = value; }
			void SetActiveSelf(bool value) { m_bActiveInHierarchy = value; }
			void SetIsStatic(bool value) { m_bIsStatic = value; }
			void SetLayer(const Layer value) { m_Layer =  value; }
			void SetScene(Scene* value) { m_pScene = value; }
			void SetTag(const tstring& value) { m_Tag = value; }
			void SetTransform(Transform* value) { m_pTransform = value; }

			bool GetActiveInHierarchy() { return m_bActiveInHierarchy; }
			bool GetActiveSelf() { return m_bActiveInHierarchy; }
			bool GetIsStatic() { return m_bIsStatic; }
			Layer GetLayer() { return m_Layer; }
			Scene& GetScene() { return *m_pScene; }
			tstring& GetTag() { return m_Tag; }
			Transform& GetTransform() { return *m_pTransform; }

			std::vector<Component*>& GetComponents() { return m_Components; }

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
			bool m_bActiveInHierarchy;

			bool m_bIsActiveSelf;

			bool m_bIsStatic;

			Layer m_Layer;

			Scene* m_pScene;

			tstring m_Tag;

			Transform* m_pTransform;

			std::vector<Component*> m_Components;
		};

	}
}

#include "GameEngine\Core\GameObject.inl"

#endif