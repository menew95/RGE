#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsHelper.h"

physx::PxTransform PhysicsModule::TransformToPxTransform(const TransformInfo& value)
{
	physx::PxTransform temp;

	temp.p.x = value._pos.x;
	temp.p.y = value._pos.y;
	temp.p.z = value._pos.z;

	temp.q.x = value._rot.x;
	temp.q.y = value._rot.y;
	temp.q.z = value._rot.z;
	temp.q.w = value._rot.w;

	return temp;
}

PhysicsModule::TransformInfo PhysicsModule::PxTransformToTransform(const physx::PxTransform& value)
{
	TransformInfo temp;

	temp._pos.x = value.p.x;
	temp._pos.y = value.p.y;
	temp._pos.z = value.p.z;

	temp._rot.x = value.q.x;
	temp._rot.y = value.q.y;
	temp._rot.z = value.q.z;
	temp._rot.w = value.q.w;

	return temp;
}

physx::PxVec3 PhysicsModule::Vector3ToPxVec3(const Math::Vector3& value)
{
	physx::PxVec3 temp;

	temp.x = value.x;
	temp.y = value.y;
	temp.z = value.z;

	return temp;
}

Math::Vector3 PhysicsModule::PxVec3ToVector3(const physx::PxVec3& value)
{
	Math::Vector3 temp;

	temp.x = value.x;
	temp.y = value.y;
	temp.z = value.z;

	return temp;
}
