#pragma once

#include "GameEngine\Core\Component\Component.h"
#include "Math\Math.h"

namespace GameEngine
{
    namespace Core
    {
		enum class Space
		{
			World = 0,
			Self = 1
		};
		class GAME_ENGINE_API Transform :
			public Component, public std::enable_shared_from_this<Transform>
		{
		public:
			Transform(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName = TEXT("Transform"));
			virtual ~Transform();

			virtual void Update();

			virtual void AddChild(std::shared_ptr<Transform>& child);
			virtual void SetParent(std::shared_ptr<Transform>& parent);
			virtual void Translate(Math::Vector3& translation, Space relativeTo = Space::Self);
			virtual void Rotate(Math::Vector3& eulers, Space relativeTo = Space::Self);
			virtual void Rotate(Math::Quaternion& quaternion, Space relativeTo = Space::Self);

			virtual void LookAt(std::shared_ptr<Transform>& target, Math::Vector3 worldUp = Math::Vector3::Up);
			virtual void LookAt(Math::Vector3& worldPosition, Math::Vector3 worldUp = Math::Vector3::Up);

			virtual void SetPositionAndRotation(Math::Vector3 position, Math::Quaternion& rotation);

			void SetWorld(Math::Matrix world);

			void SetLocal(Math::Matrix local);

			size_t GetChildCount() { return m_Childs.size(); }
			Math::Vector3& GetForward() { return m_Forward; }
			Math::Vector3& GetRight() { return m_Right; }
			Math::Vector3& GetUp() { return m_Up; }
			Math::Vector3& GetPosition() { return m_WorldPosition; }
			Math::Vector3& GetEulerAngles() { return m_WorldEulerRotation; }
			Math::Quaternion& GetRotation() { return m_WorldQuaternionRotation; }
			Math::Vector3& GetScale() { return m_WorldScale; }
			Math::Matrix& GetTM() { return m_WorldTM; }
			Math::Matrix& GetWorldTM();

			Math::Vector3& GetLocalPosition() { return m_LocalPosition; }
			Math::Vector3& GetLocalEulerAngles() { return m_LocalEulerRotation; }
			Math::Quaternion& GetLocalRotation() { return m_LocalQuaternionRotation; }
			Math::Vector3& GetLocalScale() { return m_LocalScale; }
			Math::Matrix& GetLocalTM() { return m_LocalTM; }

			std::shared_ptr<Transform>& GetParent() { return m_Parent; }
			std::vector<std::shared_ptr<Transform>>& GetChilds() { return m_Childs; }
			std::shared_ptr<Transform> GetChild(uint32 index);
		private:
			void SetLocalPosition(Math::Vector3& value);
			void SetLocalRotate(Math::Vector3& value);
			void SetLocalRotateQ(Math::Quaternion& value);
			void SetLocalScale(Math::Vector3& value);
			void SetWorldPosition(Math::Vector3& value);
			void SetWorldRotate(Math::Vector3& value);
			void SetWorldRotateQ(Math::Quaternion& value);
			void SetWorldScale(Math::Vector3& value);

			void UpdateMatrix();
			void UpdateLocalMatrix();

			void DecomposeWorldToLocal();
			void DecomposeLocalToWorld();

		protected:
			std::shared_ptr<Transform> m_Parent;
			std::vector<std::shared_ptr<Transform>> m_Childs;

			Math::Vector3 m_Forward;
			Math::Vector3 m_Right;
			Math::Vector3 m_Up;

			Math::Vector3 m_LocalScale;
			Math::Vector3 m_LocalEulerRotation;
			Math::Quaternion m_LocalQuaternionRotation;
			Math::Vector3 m_LocalPosition;

			Math::Matrix m_LocalPositionTM;
			Math::Matrix m_LocalRotateTM;
			Math::Matrix m_LocalScaleTM;

			Math::Matrix m_LocalTM;

			Math::Vector3 m_WorldScale;
			Math::Vector3 m_WorldEulerRotation;
			Math::Quaternion m_WorldQuaternionRotation;
			Math::Vector3 m_WorldPosition;

			Math::Matrix m_WorldPositionTM;
			Math::Matrix m_WorldRotateTM;
			Math::Matrix m_WorldScaleTM;

			Math::Matrix m_WorldTM;

			bool m_bIsDrity;
		};
    }
}