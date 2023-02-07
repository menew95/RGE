#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Object/Object.h"

#include "GameEngine/Core/MetaDataInfo.h"

#include "rttr/registration_friend.h"
#include "rttr/rttr_enable.h"

namespace GameEngine
{
	namespace Core
	{
		class GameObject;
		class GAME_ENGINE_API Component
			: public Object/*, public std::enable_shared_from_this<Component>*/
		{
		public:
			Component(std::shared_ptr<GameObject>& gameObject, const tstring& componentName = TEXT("Component"));
			virtual ~Component();
			
			virtual void Awake() {}
			virtual void Start() {}
			virtual void Update() {}
			virtual void LateUpdate() {}
			virtual void FixedUpdate() {}
			virtual void OnEnable() {}
			virtual void OnDisable() {}
			virtual void Render() {}

			tstring& GetTag();
			void SetTag(const tstring& value);
			
			bool GetEnable() { return m_bEnable; }
			void SetEnable(bool value);// { m_bEnable = value; }

			bool GetIsActiveAndEnabled() { return m_bIsActiveAndEnabled; }
			void SetIsActiveAndEnabled(bool value) { m_bIsActiveAndEnabled = value; }

			tstring GetGameObjectName();
			uuid GetGameObjectUUID();

			std::shared_ptr<GameObject>& GetGameObject() { return m_pGameObject; }

			std::shared_ptr<class Transform>& GetTransform();

			std::vector<std::shared_ptr<Component>>& GetComponents();

			template<typename T>
			std::shared_ptr<T>& AddComponent();

			template<typename T>
			std::shared_ptr<T> GetComponent();

			template<typename T>
			std::shared_ptr<T> GetComponentInChildren();

			template<typename T>
			std::shared_ptr<T> GetComponentInParent();

			template<typename T>
			std::vector<std::shared_ptr<T>> GetComponentsInChildren();

			template<typename T>
			std::vector<std::shared_ptr<T>> GetComponentsInParent();

			tstring GetType() { return m_TypeName; }
			void SetType(tstring value) { m_TypeName = value; }

			RTTR_ENABLE()
			RTTR_REGISTRATION_FRIEND

		protected:
			std::shared_ptr<GameObject> m_pGameObject;

			bool m_bEnable;

			bool m_bIsActiveAndEnabled;
		};
	}
}

#include "GameEngine\Core\Component\Component.inl"

#endif