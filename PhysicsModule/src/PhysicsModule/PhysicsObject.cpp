#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsObject.h"
#include "PhysicsModule\PhysicsInfo.h"
#include "PhysicsModule\Metal\RigidActor.h"
#include "PhysicsModule\Metal\Shape.h"
#include "PhysicsModule\Metal\PhysicsMaterial.h"
#include "PhysicsModule\PhysicsImpl.h"
#include "PhysicsModule\PhysicsScene.h"

PhysicsModule::PhysicsObject::PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, PhysicsMaterial* material)
	: m_pRigidActor(nullptr)
	, m_pShape(nullptr)
	, m_ColliderInfoRef(colliderInfo)
{
	m_IsVaild = CreateShape(colliderInfo, material);
	m_IsVaild = CreateColliderActor(userData, transformInfo);
}

PhysicsModule::PhysicsObject::PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* material)
	: m_pRigidActor(nullptr)
	, m_pShape(nullptr)
	, m_ColliderInfoRef(colliderInfo)
	, m_RigidInfo(rigidInfo)
{
	m_IsVaild = CreateShape(colliderInfo, material);
	m_IsVaild = CreateRigidActor(userData, transformInfo, rigidInfo);
}

PhysicsModule::PhysicsObject::~PhysicsObject()
{
	PhysicsSDK->Instance()->GetScene()->GetScene()->removeActor(*(m_pRigidActor->GetActor()));

	SAFE_DELETE(m_pRigidActor)
	SAFE_DELETE(m_pShape)
}

bool PhysicsModule::PhysicsObject::CreateShape(ColliderInfo& colliderInfo, PhysicsMaterial* physicsMaterial)
{
	m_pShape = new Shape();
	
	if (physicsMaterial == nullptr)
	{
		physicsMaterial = PhysicsSDK->GetDefaultMaterial();
	}

	switch (colliderInfo._shapeType)
	{
		case eShapeType::BOX:
		{
			m_pShape->CreateBoxShape(reinterpret_cast<BoxColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::CAPSULE:
		{
			m_pShape->CreateCapsuleShape(reinterpret_cast<CapsuleColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::SPHERE:
		{
			m_pShape->CreateSphereShape(reinterpret_cast<SphereColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::MESH:
		{
			m_pShape->CreateMeshShape(reinterpret_cast<MeshColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		case eShapeType::HEIGHTFIELD:
		{
			m_pShape->CreateHeightFieldShape(reinterpret_cast<HeightFieldColliderInfo&>(colliderInfo), physicsMaterial->GetMaterial());
			break;
		}
		default:
		{
			AssertMessageBox(false, "Shape type is unknown");
			break;
		}
	}

	return true;
}

bool PhysicsModule::PhysicsObject::CreateRigidActor(UserData& userData, TransformInfo& transformInfo, RigidInfo& rigidInfo)
{
	m_pRigidActor = new RigidActor(userData, transformInfo, m_pShape->GetShape(), rigidInfo);

	NullCheck(m_pRigidActor);

	PhysicsSDK->Instance()->GetScene()->GetScene()->addActor(*(m_pRigidActor->GetActor()));

	return true;
}

bool PhysicsModule::PhysicsObject::CreateColliderActor(UserData& userData, TransformInfo& transformInfo)
{
	m_pRigidActor = new RigidActor(userData, transformInfo, m_pShape->GetShape());

	NullCheck(m_pRigidActor);

	PhysicsSDK->Instance()->GetScene()->GetScene()->addActor(*(m_pRigidActor->GetActor()));

	return true;
}

bool PhysicsModule::PhysicsObject::OnEnable()
{
	return m_pRigidActor->OnEnable();
}

bool PhysicsModule::PhysicsObject::OnDisable()
{
	return m_pRigidActor->OnDisable();
}

void PhysicsModule::PhysicsObject::SetTransform(TransformInfo& transformInfo)
{
	NullCheck(m_pRigidActor)

	m_pRigidActor->SetTransform(transformInfo);
}

PhysicsModule::TransformInfo PhysicsModule::PhysicsObject::GetTransform()
{
	NullCheck(m_pRigidActor)

	return m_pRigidActor->GetTransform();
}

bool PhysicsModule::PhysicsObject::SetLayer(int i)
{
	NullCheck(m_pShape);

	m_pShape->SetLayer(reinterpret_cast<PxFilterData&>(PhysicsSDK->GetFilter(i)));

	return true;
}

void PhysicsModule::PhysicsObject::SetIsTrigger(bool val)
{
	NullCheck(m_pShape)

	m_pShape->SetIsTrigger(val);
}

void PhysicsModule::PhysicsObject::SetCenter(Math::Vector3 center)
{
	NullCheck(m_pShape)

	m_pShape->SetCenter(center);
}

void PhysicsModule::PhysicsObject::SetSize(Math::Vector3 size)
{
	NullCheck(m_pShape)

	m_pShape->SetSize(size);
}

void PhysicsModule::PhysicsObject::SetRadius(float radius)
{
	NullCheck(m_pShape)

	m_pShape->SetRaduis(radius);
}

void PhysicsModule::PhysicsObject::SetHeight(float height)
{
	NullCheck(m_pShape);

	m_pShape->SetHeight(height);
}

void PhysicsModule::PhysicsObject::SetAxisDirection(eAxisDirection dir)
{
	NullCheck(m_pShape);

	m_pShape->SetAxisDirection(dir);
}

bool PhysicsModule::PhysicsObject::SetMass(float mass)
{
	NullCheck(m_pShape);

	return m_pRigidActor->SetMass(mass);
}

bool PhysicsModule::PhysicsObject::SetLinearDamping(float damping)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetLinearDamping(damping);
}

bool PhysicsModule::PhysicsObject::SetAngularDamping(float damping)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetAngularDamping(damping);
}

bool PhysicsModule::PhysicsObject::SetGravity(bool flag)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetGravity(flag);
}

bool PhysicsModule::PhysicsObject::SetKinematic(bool flag)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetKinematic(flag);
}

bool PhysicsModule::PhysicsObject::SetConstraints(Constraints& constraints)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetConstraints(constraints);
}

bool PhysicsModule::PhysicsObject::SetCollisionDetection(eCollisionDetection& collisionDetection)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetCollisionDetection(collisionDetection);
}

bool PhysicsModule::PhysicsObject::SetLinearVelocity(Math::Vector3 velocity)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetLinearVelocity(velocity);
}

bool PhysicsModule::PhysicsObject::SetAngularVelocity(Math::Vector3 velocity)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetAngularVelocity(velocity);
}

bool PhysicsModule::PhysicsObject::SetMaxLinearVelocity(float val)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetMaxLinearVelocity(val);
}

bool PhysicsModule::PhysicsObject::SetMaxAngularVelocity(float val)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->SetMaxAngularVelocity(val);
}

bool PhysicsModule::PhysicsObject::AddForce(Math::Vector3 vector, int forceMode)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->AddForce(vector, forceMode);
}

bool PhysicsModule::PhysicsObject::AddTorque(Math::Vector3 torque, int forceMode)
{
	NullCheck(m_pRigidActor);

	return m_pRigidActor->AddTorque(torque, forceMode);
}