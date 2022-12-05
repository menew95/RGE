#pragma once
#include <PhysX\PxSimulationEventCallback.h>

#include "PhysicsModule\PhysicsInfo.h"

using namespace physx;


namespace PhysicsModule
{
	class BaseEventCallBack :
		public PxSimulationEventCallback
	{
	public:
		BaseEventCallBack() = default;
		~BaseEventCallBack();

		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
		virtual void onWake(PxActor** actors, PxU32 count) override;
		virtual void onSleep(PxActor** actors, PxU32 count) override;
		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
		virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

		virtual std::vector<CollisionEvent>& GetCollisionEvents() { return m_CollisionEvents; }
		std::vector<TriggerEvent>& GetTriggerEvents() { return m_TriggerEvents; }

		void Tick();
	private:
		std::vector<CollisionEvent> m_CollisionEvents;
		std::vector<TriggerEvent> m_TriggerEvents;
	};
}