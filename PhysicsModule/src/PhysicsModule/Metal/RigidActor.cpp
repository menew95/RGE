#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsImpl.h"
#include "PhysicsModule\Metal\RigidActor.h"
#include "PhysicsModule\Metal\Shape.h"

#define CHECK_DYNAMIC() NullCheck(m_pRigidActor) \
if(!IsDynamic()) { return false; }


PhysicsModule::RigidActor::RigidActor(UserData userData, RigidInfo& rigidInfo, physx::PxRigidActor* rigidActor)
	: m_pRigidActor(rigidActor)
	, m_RigidInfo(rigidInfo)
	, m_UserData(userData)
{

}

PhysicsModule::RigidActor::RigidActor(UserData userData, TransformInfo& transformInfo, physx::PxShape* shape)
	: m_UserData(userData)
{
	physx::PxTransform pose;

	pose.p.x = transformInfo._pos.x;
	pose.p.y = transformInfo._pos.y;
	pose.p.z = transformInfo._pos.z;

	pose.q.x = transformInfo._rot.x;
	pose.q.y = transformInfo._rot.y;
	pose.q.z = transformInfo._rot.z;
	pose.q.w = transformInfo._rot.w;

	CreateStatic(pose, shape);
}

PhysicsModule::RigidActor::RigidActor(UserData userData, TransformInfo& transformInfo, physx::PxShape* shape, RigidInfo& rigidInfo)
	: m_RigidInfo(rigidInfo)
	, m_UserData(userData)
{
	physx::PxTransform pose;

	pose.p.x = transformInfo._pos.x;
	pose.p.y = transformInfo._pos.y;
	pose.p.z = transformInfo._pos.z;

	pose.q.x = transformInfo._rot.x;
	pose.q.y = transformInfo._rot.y;
	pose.q.z = transformInfo._rot.z;
	pose.q.w = transformInfo._rot.w;

	CreateDynamic(rigidInfo, pose, shape);
}

PhysicsModule::RigidActor::~RigidActor()
{

}

void PhysicsModule::RigidActor::Release()
{
	SAFE_RELEASE(m_pRigidActor)
}

void PhysicsModule::RigidActor::CreateStatic(PxTransform& transform, physx::PxShape* shape)
{

	//_pxRigidActor = physx::PxCreateStatic(*(PhysicsSDK->GetPhysics()), transform, *shape);

	m_pRigidActor = PhysicsSDK->GetPhysics()->createRigidStatic(transform);

	m_pRigidActor->attachShape(*shape);

	m_pRigidActor->userData = &m_UserData;

	m_eRigidType = eRigidType::Static;
}

void PhysicsModule::RigidActor::CreateDynamic(RigidInfo& rigidInfo, PxTransform& transform, physx::PxShape* shape)
{
	m_pRigidActor = PhysicsSDK->GetPhysics()->createRigidDynamic(transform);

	m_pRigidActor->attachShape(*shape);

	m_pRigidActor->userData = &m_UserData;

	m_eRigidType = eRigidType::Dynamic;

	InitRigidBody(rigidInfo);
}


bool PhysicsModule::RigidActor::InitRigidBody(RigidInfo& rigidInfo)
{
	CHECK_DYNAMIC();

	PxRigidDynamic* _actor = m_pRigidActor->is<PxRigidDynamic>();

	assert(_actor != nullptr);

	_actor->setMass(rigidInfo._mass);
	_actor->setLinearDamping(rigidInfo._drag);
	_actor->setAngularDamping(rigidInfo._angularDrag);
	_actor->setActorFlag(PxActorFlag::Enum::eDISABLE_GRAVITY, !rigidInfo._useGravity);

	PxRigidBodyFlags rigidBodyFlag;

	if (rigidInfo._isKinematic)
		rigidBodyFlag |= PxRigidBodyFlag::Enum::eKINEMATIC;

	switch (rigidInfo._collisionDetection)
	{
		case eCollisionDetection::Continuous:
		{
			rigidBodyFlag |= PxRigidBodyFlag::eENABLE_CCD;
			break;
		}
		case eCollisionDetection::ContinuousSpeculative:
		{
			rigidBodyFlag |= PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD;
			break;
		}
		case eCollisionDetection::Discrete:
		{
			break;
		}
	}

	_actor->setRigidBodyFlags(rigidBodyFlag);

	PxRigidDynamicLockFlags lockFlags;

	SetConstraints(m_RigidInfo._constraints);

	return true;
}

bool PhysicsModule::RigidActor::ChangeToCollider(TransformInfo& transformInfo, PxShape* shape)
{
	SAFE_RELEASE(m_pRigidActor);

	physx::PxTransform pose;

	pose.p.x = transformInfo._pos.x;
	pose.p.y = transformInfo._pos.y;
	pose.p.z = transformInfo._pos.z;

	pose.q.x = transformInfo._rot.x;
	pose.q.y = transformInfo._rot.y;
	pose.q.z = transformInfo._rot.z;
	pose.q.w = transformInfo._rot.w;

	CreateStatic(pose, shape);

	NullCheck(m_pRigidActor);

	return true;
}

bool PhysicsModule::RigidActor::ChangeToRigidBody(TransformInfo& transformInfo, PxShape* shape, RigidInfo& rigidInfo)
{
	SAFE_RELEASE(m_pRigidActor);

	physx::PxTransform pose;

	pose.p.x = transformInfo._pos.x;
	pose.p.y = transformInfo._pos.y;
	pose.p.z = transformInfo._pos.z;

	pose.q.x = transformInfo._rot.x;
	pose.q.y = transformInfo._rot.y;
	pose.q.z = transformInfo._rot.z;
	pose.q.w = transformInfo._rot.w;

	CreateDynamic(rigidInfo, pose, shape);

	NullCheck(m_pRigidActor);

	return true;
}

bool PhysicsModule::RigidActor::IsDynamic()
{
	return (m_eRigidType == eRigidType::Dynamic);
}

bool PhysicsModule::RigidActor::AddForce(Math::Vector3 vector, int forceMode)
{
	CHECK_DYNAMIC();
	
	((PxRigidBody*)m_pRigidActor)->addForce(PxVec3(vector.x, vector.y, vector.z), (PxForceMode::Enum)forceMode);

	return true;
}

bool PhysicsModule::RigidActor::AddTorque(Math::Vector3 torque, int forceMode)
{
	CHECK_DYNAMIC();

	((PxRigidBody*)m_pRigidActor)->addTorque(PxVec3(torque.x, torque.y, torque.z), (PxForceMode::Enum)forceMode);

	return true;
}

bool PhysicsModule::RigidActor::SetTransform(TransformInfo& transform)
{
	physx::PxTransform pose;

	pose.p.x = transform._pos.x;
	pose.p.y = transform._pos.y;
	pose.p.z = transform._pos.z;

	pose.q.x = transform._rot.x;
	pose.q.y = transform._rot.y;
	pose.q.z = transform._rot.z;
	pose.q.w = transform._rot.w;

	((physx::PxRigidDynamic*)m_pRigidActor)->setGlobalPose(pose);

	return true;
}

bool PhysicsModule::RigidActor::SetIsTrigger(bool flag)
{
	physx::PxShape** oldShape = new physx::PxShape*;

	m_pRigidActor->getShapes(oldShape, 1);

	if (oldShape != nullptr)
	{
		(*oldShape)->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !flag);
		(*oldShape)->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, flag);

		delete oldShape;
		return true;
	}

	delete oldShape;

	return false;
}

PhysicsModule::TransformInfo PhysicsModule::RigidActor::GetTransform()
{
	TransformInfo transformInfo;

	auto pose = m_pRigidActor->getGlobalPose();

	transformInfo._pos.x = pose.p.x;
	transformInfo._pos.y = pose.p.y;
	transformInfo._pos.z = pose.p.z;

	transformInfo._rot.x = pose.q.x;
	transformInfo._rot.y = pose.q.y;
	transformInfo._rot.z = pose.q.z;
	transformInfo._rot.w = pose.q.w;

	return transformInfo;
}

bool PhysicsModule::RigidActor::SetLinearDamping(float damping)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setLinearDamping(damping);

	return true;
}

bool PhysicsModule::RigidActor::SetAngularDamping(float damping)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setAngularDamping(damping);

	return true;
}

bool PhysicsModule::RigidActor::SetLinearVelocity(Math::Vector3 velocity)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));

	return true;
}

bool PhysicsModule::RigidActor::SetAngularVelocity(Math::Vector3 velocity)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setAngularVelocity(PxVec3(velocity.x, velocity.y, velocity.z));

	return true;
}

bool PhysicsModule::RigidActor::SetMaxLinearVelocity(float vel)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setMaxLinearVelocity(vel);

	return true;
}

bool PhysicsModule::RigidActor::SetMaxAngularVelocity(float vel)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setMaxAngularVelocity(vel);

	return true;
}

bool PhysicsModule::RigidActor::SetGravity(bool flag)
{
	CHECK_DYNAMIC();

	m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, flag);

	return true;
}

bool PhysicsModule::RigidActor::SetKinematic(bool flag)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, flag);

	return true;
}

bool PhysicsModule::RigidActor::SetMass(float mass)
{
	CHECK_DYNAMIC();

	((physx::PxRigidBody*)m_pRigidActor)->setMass(mass);

	return true;
}

bool PhysicsModule::RigidActor::SetConstraints(Constraints& constraints)
{
	CHECK_DYNAMIC();

	PxRigidDynamicLockFlags flags;


	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, constraints._position._x);
	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, constraints._position._y);
	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, constraints._position._z);
	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, constraints._rotation._x);
	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, constraints._rotation._y);
	((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, constraints._rotation._z);

	//(constraints._position._x == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_X : flags &= ~PxRigidDynamicLockFlag::eLOCK_LINEAR_X;
	//(constraints._position._y == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Y : flags &= ~PxRigidDynamicLockFlag::eLOCK_LINEAR_Y;
	//(constraints._position._z == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_LINEAR_Z : flags &= ~PxRigidDynamicLockFlag::eLOCK_LINEAR_Z;
	//(constraints._rotation._x == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_X : flags &= ~PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
	//(constraints._rotation._y == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y : flags &= ~PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
	//(constraints._rotation._z == true) ? flags |= PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z : flags &= ~PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

	//((physx::PxRigidDynamic*)m_pRigidActor)->setRigidDynamicLockFlags(flags);

	return true;
}

bool PhysicsModule::RigidActor::SetCollisionDetection(eCollisionDetection collisionDetection)
{
	CHECK_DYNAMIC();

	switch (collisionDetection)
	{
	case PhysicsModule::eCollisionDetection::Discrete:
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_CCD, false);
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_SPECULATIVE_CCD, false);
		break;
	case PhysicsModule::eCollisionDetection::Continuous:
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_CCD, true);
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_SPECULATIVE_CCD, false);
		break;
	case PhysicsModule::eCollisionDetection::ContinuousSpeculative:
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_CCD, false);
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_CCD, false);
		m_pRigidActor->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::Enum::eENABLE_SPECULATIVE_CCD, true);
		break;
	default:
		AssertMessageBox(false, "CollisionDetection value is unknown");
		return false;
		break;
	}

	return true;
}

bool PhysicsModule::RigidActor::OnEnable()
{
	NullCheck(m_pRigidActor);

	m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);

	return true;
}

bool PhysicsModule::RigidActor::OnDisable()
{
	NullCheck(m_pRigidActor);

	m_pRigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);

	return true;
}
