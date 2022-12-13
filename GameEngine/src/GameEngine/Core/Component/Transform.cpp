#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Component\Transform.h"

namespace GameEngine
{
	namespace Core
	{

		Transform::Transform(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName /*= TEXT("Transform")*/)
			: Component(gameObject, componentName)
		{

		}

		Transform::~Transform()
		{

		}

		void Transform::Update()
		{
			__super::Update();

			if (m_bIsDrity)
			{
				UpdateMatrix();
			}
		}

		void Transform::SetParent(std::shared_ptr<Transform>& parent)
		{
			if (m_Parent == parent)
				return;

			DecomposeWorldToLocal();
		}

		void Transform::Translate(Math::Vector3& translation, Space relativeTo /*= Space::Self*/)
		{
			switch (relativeTo)
			{
				case GameEngine::Core::Space::World:
				{
					m_WorldPosition += translation;

					SetWorldPosition(m_WorldPosition);
					break;
				}
				case GameEngine::Core::Space::Self:
				{
					m_LocalPosition += translation;

					SetLocalPosition(translation);
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}

		void Transform::Rotate(Math::Vector3& eulers, Space relativeTo /*= Space::Self*/)
		{
			switch (relativeTo)
			{
				case GameEngine::Core::Space::World:
				{
					m_WorldEulerRotation += eulers;

					SetWorldRotate(m_WorldEulerRotation);
					break;
				}
				case GameEngine::Core::Space::Self:
				{
					m_LocalEulerRotation += eulers;

					SetLocalRotate(m_LocalEulerRotation);
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}

		void Transform::Rotate(Math::Quaternion& quaternion, Space relativeTo /*= Space::Self*/)
		{
			switch (relativeTo)
			{
				case GameEngine::Core::Space::World:
				{
					m_WorldQuaternionRotation *= quaternion;

					SetWorldRotateQ(m_WorldQuaternionRotation);
					break;
				}
				case GameEngine::Core::Space::Self:
				{
					m_LocalQuaternionRotation *= quaternion;

					SetLocalRotateQ(m_LocalQuaternionRotation);
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}

		void Transform::LookAt(std::shared_ptr<Transform>& target, Math::Vector3 worldUp /*= Math::Vector3::Up*/)
		{
			assert(target != nullptr);

			LookAt(target->GetPosition(), worldUp);
		}

		void Transform::LookAt(Math::Vector3& worldPosition, Math::Vector3 worldUp /*= Math::Vector3::Up*/)
		{
			m_WorldRotateTM = Math::Matrix::CreateWorld(m_WorldPosition, worldPosition, worldUp);

			DecomposeWorldToLocal();
		}

		void Transform::SetPositionAndRotation(Math::Vector3 position, Math::Quaternion& rotation)
		{
			m_WorldPosition = position;

			m_WorldQuaternionRotation = rotation;

			m_WorldEulerRotation = rotation.ToEuler();

			m_WorldPositionTM = Math::Matrix::CreateTranslation(position);

			m_WorldRotateTM = Math::Matrix::CreateFromQuaternion(rotation);

			DecomposeWorldToLocal();
		}

		Math::Matrix& Transform::GetWorldTM()
		{
			if (m_bIsDrity)
			{
				UpdateMatrix();
			}

			return m_WorldTM;
		}

		std::shared_ptr<Transform> Transform::GetChild(uint32 index)
		{
			if (index < m_Childs.size())
			{
				return m_Childs[index];
			}

			return nullptr;
		}

		/*Math::Matrix& Transform::GetTM()
		{
			if (m_bIsDrity)
			{
				UpdateMatrix();
			}

			return m_WorldTM;
		}

		Math::Matrix& Transform::GetLocalTM()
		{
			if (m_bIsDrity)
			{
				UpdateMatrix();
			}

			return m_LocalTM;
		}*/

		void Transform::SetLocalPosition(Math::Vector3& value)
		{
			m_LocalPosition = value;

			m_LocalPositionTM = Math::Matrix::CreateTranslation(value);

			DecomposeLocalToWorld();
		}

		void Transform::SetLocalRotate(Math::Vector3& value)
		{
			m_LocalEulerRotation = value;

			m_LocalQuaternionRotation = Math::Quaternion::CreateFromYawPitchRoll(value);

			m_LocalRotateTM = Math::Matrix::CreateFromQuaternion(m_LocalQuaternionRotation);

			DecomposeLocalToWorld();
		}

		void Transform::SetLocalRotateQ(Math::Quaternion& value)
		{
			m_LocalQuaternionRotation = value;

			m_LocalEulerRotation = value.ToEuler();

			m_LocalRotateTM = Math::Matrix::CreateFromQuaternion(value);

			DecomposeLocalToWorld();
		}

		void Transform::SetLocalScale(Math::Vector3& value)
		{
			m_LocalScale = value;

			m_LocalScaleTM = Math::Matrix::CreateScale(value);

			DecomposeLocalToWorld();
		}

		void Transform::SetWorldPosition(Math::Vector3& value)
		{
			m_WorldPosition = value;

			m_WorldPositionTM = Math::Matrix::CreateTranslation(value);

			DecomposeWorldToLocal();
		}

		void Transform::SetWorldRotate(Math::Vector3& value)
		{
			m_WorldEulerRotation = value;

			m_WorldQuaternionRotation = Math::Quaternion::CreateFromYawPitchRoll(value);

			m_WorldRotateTM = Math::Matrix::CreateFromQuaternion(m_WorldQuaternionRotation);

			DecomposeWorldToLocal();
		}

		void Transform::SetWorldRotateQ(Math::Quaternion& value)
		{
			m_WorldQuaternionRotation = value;

			m_WorldEulerRotation = value.ToEuler();

			m_WorldRotateTM = Math::Matrix::CreateFromQuaternion(value);

			DecomposeWorldToLocal();
		}

		void Transform::SetWorldScale(Math::Vector3& value)
		{
			m_WorldScale = value;

			m_WorldScaleTM = Math::Matrix::CreateScale(value);

			DecomposeWorldToLocal();
		}

		void Transform::UpdateMatrix()
		{
			if (m_Parent != nullptr)
			{
				auto& _parentTM = m_Parent->GetWorldTM();

				UpdateLocalMatrix();

				m_WorldTM = m_LocalTM * _parentTM;

				m_WorldPositionTM = m_LocalPositionTM * _parentTM;
				m_WorldRotateTM = m_LocalRotateTM * _parentTM;
				m_WorldScaleTM = m_LocalScaleTM * _parentTM;

				m_WorldTM.Decompose(m_WorldScale, m_WorldQuaternionRotation, m_WorldPosition);

				m_WorldEulerRotation = m_WorldQuaternionRotation.ToEuler();
			}
			else
			{
				UpdateLocalMatrix();

				m_WorldTM = m_LocalTM;

				m_WorldPositionTM = m_LocalPositionTM;
				m_WorldRotateTM = m_LocalRotateTM;
				m_WorldScaleTM = m_LocalScaleTM;

				m_WorldPosition = m_LocalPosition;
				m_WorldEulerRotation = m_WorldEulerRotation;
				m_WorldQuaternionRotation = m_LocalQuaternionRotation;
				m_WorldScale = m_LocalScale;
			}

			m_Forward = m_WorldRotateTM.Forward();
			m_Up = m_WorldRotateTM.Up();
			m_Right = m_WorldRotateTM.Right();

			m_bIsDrity = false;

			for (auto& child : m_Childs)
			{
				child->UpdateMatrix();
			}
		}

		void Transform::UpdateLocalMatrix()
		{
			m_LocalPositionTM._41 = m_LocalPosition.x;
			m_LocalPositionTM._42 = m_LocalPosition.y;
			m_LocalPositionTM._43 = m_LocalPosition.z;

			m_LocalEulerRotation = m_LocalQuaternionRotation.ToEuler();

			m_LocalRotateTM = Math::Matrix::CreateFromQuaternion(m_LocalQuaternionRotation);

			m_LocalScaleTM._11 = m_LocalScale.x;
			m_LocalScaleTM._22 = m_LocalScale.y;
			m_LocalScaleTM._33 = m_LocalScale.z;

			m_LocalTM = m_LocalScaleTM * m_LocalRotateTM * m_LocalPositionTM;
		}

		void Transform::DecomposeWorldToLocal()
		{
			m_WorldTM = m_WorldScaleTM * m_WorldRotateTM * m_WorldPositionTM;

			if (m_Parent != nullptr)
			{
				Math::Matrix _parentInv = m_Parent->GetTM().Invert();

				m_LocalTM = m_WorldTM * _parentInv;

				m_LocalTM.Decompose(m_LocalScale, m_LocalQuaternionRotation, m_LocalPosition);

				m_LocalEulerRotation = m_LocalQuaternionRotation.ToEuler();

				m_LocalPositionTM = Math::Matrix::CreateTranslation(m_LocalPosition);
				m_LocalRotateTM = Math::Matrix::CreateFromQuaternion(m_LocalQuaternionRotation);
				m_LocalScaleTM = Math::Matrix::CreateScale(m_LocalScale);
			}
			else
			{
				m_LocalScale = m_WorldScale;
				m_LocalEulerRotation = m_WorldEulerRotation;
				m_LocalQuaternionRotation = m_WorldQuaternionRotation;
				m_LocalPosition = m_WorldPosition;

				m_LocalTM = m_WorldTM;

				m_LocalPositionTM = m_WorldPositionTM;
				m_LocalRotateTM = m_WorldRotateTM;
				m_LocalScaleTM = m_WorldScaleTM;
			}

			m_Forward = m_WorldRotateTM.Forward();
			m_Up = m_WorldRotateTM.Up();
			m_Right = m_WorldRotateTM.Right();
		}

		void Transform::DecomposeLocalToWorld()
		{
			m_LocalTM = m_LocalScaleTM * m_LocalRotateTM * m_LocalPositionTM;

			if (m_Parent != nullptr)
			{
				auto& _parentTM = m_Parent->GetTM();

				m_WorldTM = m_LocalTM * _parentTM;

				m_WorldTM.Decompose(m_WorldScale, m_WorldQuaternionRotation, m_WorldPosition);

				m_WorldEulerRotation = m_WorldQuaternionRotation.ToEuler();

				m_WorldPositionTM = Math::Matrix::CreateTranslation(m_WorldPosition);
				m_WorldRotateTM = Math::Matrix::CreateFromQuaternion(m_WorldQuaternionRotation);
				m_WorldScaleTM = Math::Matrix::CreateScale(m_WorldScale);
			}
			else
			{
				m_WorldPosition = m_LocalPosition;
				m_WorldEulerRotation = m_WorldEulerRotation;
				m_WorldQuaternionRotation = m_LocalQuaternionRotation;
				m_WorldScale = m_LocalScale;

				m_WorldTM = m_LocalTM;

				m_WorldPositionTM = m_LocalPositionTM;
				m_WorldRotateTM = m_LocalRotateTM;
				m_WorldScaleTM = m_LocalScaleTM;
			}

			m_Forward = m_WorldRotateTM.Forward();
			m_Up = m_WorldRotateTM.Up();
			m_Right = m_WorldRotateTM.Right();
		}

	}
}