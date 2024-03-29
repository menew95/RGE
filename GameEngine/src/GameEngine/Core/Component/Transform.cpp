#include "GameEngine\GameEnginePCH.h"
#include "GameEngine\Core\Component\Transform.h"

#include <rttr/registration.h>

using namespace rttr;

std::shared_ptr<GameEngine::Core::Component> conv_func_to_base(std::shared_ptr<GameEngine::Core::Transform> value, bool& ok)
{
	ok = true;
	return std::static_pointer_cast<GameEngine::Core::Component>(value);
}

std::shared_ptr<GameEngine::Core::Transform> conv_func_to_derived(std::shared_ptr<GameEngine::Core::Component> value, bool& ok)
{
	ok = true;
	return std::static_pointer_cast<GameEngine::Core::Transform>(value);
}

std::vector<std::shared_ptr<GameEngine::Core::Component>> conv_func_to_array_base(std::vector<std::shared_ptr<GameEngine::Core::Transform>> value, bool& ok)
{
	ok = true;
	std::vector<std::shared_ptr<GameEngine::Core::Component>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<GameEngine::Core::Component>(_ptr));
	}

	return _ret;
}


std::vector<std::shared_ptr<GameEngine::Core::Transform>> conv_func_to_array_derived(std::vector<std::shared_ptr<GameEngine::Core::Component>> value, bool& ok)
{
	ok = true;
	std::vector<std::shared_ptr<GameEngine::Core::Transform>> _ret;

	for (auto& _ptr : value)
	{
		_ret.push_back(std::static_pointer_cast<GameEngine::Core::Transform>(_ptr));
	}

	return _ret;
}

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::Transform>("Transform")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_Parent", &GameEngine::Core::Transform::GetParent, &GameEngine::Core::Transform::SetParent)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetGameObjectName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Component),
		metadata(GameEngine::Core::ObjectType::Component, "Transform")
	)
	.property("m_Childs", &GameEngine::Core::Transform::m_Childs)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckChildVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetGameObjectName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Component),
		metadata(GameEngine::Core::ObjectType::Component, "Transform")
	)
	.property("m_LocalTM", &GameEngine::Core::Transform::GetLocalTM, &GameEngine::Core::Transform::SetLocalTM)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::MATRIX)
	)
	.property("m_WorldTM", &GameEngine::Core::Transform::GetWorldTM, &GameEngine::Core::Transform::SetWorldTM)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::MATRIX)
	)
	.property("m_bIsDirty", &GameEngine::Core::Transform::m_bIsDrity)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::BOOL)
	)
	.method("GetParent", &GameEngine::Core::Transform::GetParent)
	.method("CheckVaild", &GameEngine::Core::Transform::CheckVaild)
	.method("CheckChildVaild", &GameEngine::Core::Transform::CheckChildVaild);

	type::register_converter_func(conv_func_to_base);
	type::register_converter_func(conv_func_to_derived);
	type::register_converter_func(conv_func_to_array_base);
	type::register_converter_func(conv_func_to_array_derived);
}

namespace GameEngine
{
	namespace Core
	{

		Transform::Transform(std::shared_ptr<class GameObject>& gameObject, const tstring& componentName /*= TEXT("Transform")*/)
			: Component(gameObject, componentName)
			, m_Forward(Math::Vector3::Forward)
			, m_Right(Math::Vector3::Right)
			, m_Up(Math::Vector3::Up)
			, m_LocalScale(Math::Vector3::One)
			, m_LocalEulerRotation(Math::Vector3::Zero)
			, m_LocalQuaternionRotation(Math::Quaternion::Identity)
			, m_LocalPosition(Math::Vector3::Zero)
			, m_LocalPositionTM(Math::Matrix::Identity)
			, m_LocalRotateTM(Math::Matrix::Identity)
			, m_LocalScaleTM(Math::Matrix::Identity)
			, m_LocalTM(Math::Matrix::Identity)
			, m_WorldScale(Math::Vector3::One)
			, m_WorldEulerRotation(Math::Vector3::Zero)
			, m_WorldQuaternionRotation(Math::Quaternion::Identity)
			, m_WorldPosition(Math::Vector3::Zero)
			, m_WorldPositionTM(Math::Matrix::Identity)
			, m_WorldRotateTM(Math::Matrix::Identity)
			, m_WorldScaleTM(Math::Matrix::Identity)
			, m_WorldTM(Math::Matrix::Identity)
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

		void Transform::AddChild(std::shared_ptr<Transform>& child)
		{
			auto _find = std::find(m_Childs.begin(), m_Childs.end(), child);

			if (_find != m_Childs.end())
				return;

			m_Childs.push_back(child);
		}

		void Transform::SetParent(std::shared_ptr<Transform> parent)
		{
			if (m_Parent == parent)
				return;

			m_Parent = parent;

			auto _this = shared_from_this();

			parent->AddChild(_this);

			DecomposeWorldToLocal();
		}

		void Transform::Translate(Math::Vector3& translation, Space relativeTo /*= Space::Self*/)
		{
			m_bIsDrity = true;

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
			m_bIsDrity = true;

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
			m_bIsDrity = true;

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

		void Transform::Scale(Math::Vector3& scale, Space relativeTo /*= Space::Self*/)
		{
			m_bIsDrity = true;

			switch (relativeTo)
			{
				case GameEngine::Core::Space::World:
				{
					m_WorldScale *= scale;

					SetWorldScale(m_WorldScale);
					break;
				}
				case GameEngine::Core::Space::Self:
				{
					m_LocalScale *= scale;

					SetLocalScale(m_LocalScale);
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

		void Transform::SetWorldTM(Math::Matrix world)
		{
			m_WorldTM = world;

			world.Decompose(m_WorldScale, m_WorldQuaternionRotation, m_WorldPosition);

			m_WorldPositionTM = Math::Matrix::CreateTranslation(m_WorldPosition);

			m_WorldEulerRotation = m_WorldQuaternionRotation.ToEuler();

			m_WorldRotateTM = Math::Matrix::CreateFromQuaternion(m_WorldQuaternionRotation);

			m_WorldScaleTM = Math::Matrix::CreateScale(m_WorldScale);

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

		Math::Matrix Transform::GetWorldTM()
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
				auto _parentTM = m_Parent->GetWorldTM();

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

		void Transform::SetLocalTM(Math::Matrix local)
		{
			m_LocalTM = local;

			m_LocalTM.Decompose(m_LocalScale, m_LocalQuaternionRotation, m_LocalPosition);

			m_LocalPositionTM = Math::Matrix::CreateTranslation(m_LocalPosition);

			m_LocalEulerRotation = m_LocalQuaternionRotation.ToEuler();

			m_LocalRotateTM = Math::Matrix::CreateFromQuaternion(m_LocalQuaternionRotation);

			m_LocalScaleTM = Math::Matrix::CreateScale(m_LocalScale);

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