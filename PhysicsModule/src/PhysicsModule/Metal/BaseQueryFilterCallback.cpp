#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\Metal\BaseQueryFilterCallback.h"

namespace PhysicsModule
{
	physx::PxQueryHitType::Enum BaseQueryFilterCallback::preFilter(const physx::PxFilterData& filterData, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
	{
		
		return physx::PxQueryHitType::Enum();
	}

	physx::PxQueryHitType::Enum BaseQueryFilterCallback::postFilter(const physx::PxFilterData& filterData, const physx::PxQueryHit& hit)
	{

		return physx::PxQueryHitType::Enum();
	}
}