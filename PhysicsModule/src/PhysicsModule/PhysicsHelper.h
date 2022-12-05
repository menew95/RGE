#pragma once

namespace PhysicsModule
{
	physx::PxTransform TransformToPxTransform(const TransformInfo& value);
	TransformInfo PxTransformToTransform(const physx::PxTransform& value);

	physx::PxVec3 Vector3ToPxVec3(const Math::Vector3& value);
	Math::Vector3 PxVec3ToVector3(const physx::PxVec3& value);
}