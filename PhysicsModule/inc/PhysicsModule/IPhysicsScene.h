#pragma once

#include "PhysicsModule\PhysicsInfo.h"
#include "Common.h"

namespace PhysicsModule
{
	class PhysicsObject;

	class IPhysicsScene
	{
	public:

		/// <summary>
		/// Initalize scene setting
		/// </summary>
		/// <param name="sceneInfo"></param>
		virtual void InitScene(PhysicsInfo& physicsInfo) abstract;

		/// <summary>
		/// Create physics object in current scene
		/// </summary>
		/// <returns>object uid</returns>
		virtual UID CreatePhysicObject() abstract;

		/// <summary>
		/// Delete physics object in current scene
		/// </summary>
		/// <param name="object">object uid</param>
		/// <returns>success</returns>
		virtual bool DeletePhysicsObject(UID id) abstract;

		/// <summary>
		/// Simulate scene
		/// </summary>
		/// <param name="elapseTime">elapse time</param>
		virtual void Simulate(float elapseTime) abstract;

		//virtual bool Raycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance, PxRaycastBuffer& buffer, uint32 layerMask) abstract;
	
		virtual std::vector<CollisionEvent>& GetCollisionEvents() abstract;

		virtual std::vector<TriggerEvent>& GetTriggerEvents() abstract;
	};
}