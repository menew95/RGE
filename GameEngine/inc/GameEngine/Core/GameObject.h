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
			public Object, public std::enable_shared_from_this<GameObject>
		{
		public:
			GameObject(const tstring& gameObjectName = TEXT("GameObject"));

			bool CompareTag(const tstring& value) { return value == m_Tag; }

			void SetActiveInHierarchy(bool value);
			void SetActiveSelf(bool value);
			void SetIsStatic(bool value) { m_bIsStatic = value; }
			void SetLayer(const Layer value) { m_Layer =  value; }
			void SetTag(const tstring& value) { m_Tag = value; }

			bool GetActiveInHierarchy() { return m_bActiveInHierarchy; }
			bool GetActiveSelf() { return m_bActiveInHierarchy; }
			bool GetIsStatic() { return m_bIsStatic; }
			Layer GetLayer() { return m_Layer; }
			tstring& GetTag() { return m_Tag; }

			std::weak_ptr<Scene>& GetScene() { return m_pScene; }
			std::shared_ptr<Transform>& GetTransform() { return m_pTransform; }

			std::vector<std::shared_ptr<Component>>& GetComponents() { return m_Components; }

			template<typename T>
			std::shared_ptr<T>& AddComponent();

			template<typename T>
			std::shared_ptr<T>& GetComponent();

			template<typename T>
			std::shared_ptr<T>& GetComponentInChildren();

			template<typename T>
			std::shared_ptr<T>& GetComponentInParent();

			template<typename T>
			std::vector<std::shared_ptr<T>>& GetComponentsInChildren();

			template<typename T>
			std::vector<std::shared_ptr<T>>& GetComponentsInParent();

		protected:
			bool m_bActiveInHierarchy;

			bool m_bIsActiveSelf;

			bool m_bIsStatic;

			Layer m_Layer;

			std::weak_ptr<Scene> m_pScene;

			tstring m_Tag;

			std::shared_ptr<Transform> m_pTransform;

			std::vector<std::shared_ptr<Component>> m_Components;
		};

	}
}

#include "GameEngine\Core\GameObject.inl"

#endif