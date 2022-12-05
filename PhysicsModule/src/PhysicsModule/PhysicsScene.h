#pragma once

#include <PhysX\PxPhysicsAPI.h>

#include "Common.h"

#include "PhysicsModule\IPhysicsScene.h"

using namespace physx;

namespace PhysicsModule
{
	class PhysicsObject;

	class PhysicsScene : public IPhysicsScene
	{
	public:
		PhysicsScene(physx::PxPhysics* physics, physx::PxCpuDispatcher* dispatcher);
		~PhysicsScene();

		/// <summary>
		/// Initalize scene setting
		/// </summary>
		/// <param name="sceneInfo"></param>
		void InitScene(PhysicsInfo& physicsInfo);

		/// <summary>
		/// Create physics object in current scene
		/// </summary>
		/// <returns>object uid</returns>
		UID	CreatePhysicObject();

		/// <summary>
		/// Delete physics object in current scene
		/// </summary>
		/// <param name="object">object uid</param>
		/// <returns>success</returns>
		bool DeletePhysicsObject(UID id);

		/// <summary>
		/// Simulate scene
		/// </summary>
		/// <param name="elapseTime">elapse time</param>
		void Simulate(float elapseTime);


		/// @brief Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		///	or via a custom user callback implementation inheriting from PxRaycastCallback.
		/// @param origin Origin of the ray
		/// @param unitDir Normalized Direction of the ray
		/// @param reycastHitInfo Raycast hit buffer
		/// @param maxDistance Length of the ray, Has to be in the [0, inf] range
		/// @return If ray hit object in the scene, return object information
		bool Raycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance, PxRaycastBuffer& buffer, uint32 layerMask);

		std::vector<CollisionEvent>& GetCollisionEvents();

		std::vector<TriggerEvent>& GetTriggerEvents();

		bool SetEventCallback(PxSimulationEventCallback* eventCallback);
		void AddAggregate(PxAggregate& aggregate);
		void RemoveAggregate(PxAggregate& aggregate, bool wakeOnLostTouch = true);
		void AddCollection(const PxCollection& collection);
		void ShiftOrigin(const PxVec3& shift);
		PxPvdSceneClient*	GetScenePvdClient();

		auto	GetScene() { return m_pScene; }

	private:
		void AddActors(PxActor* const* actors, PxU32 nbActors);
		void AddActors(const PxPruningStructure& pruningStructure);
		void RemoveActor(PxActor& actor, bool wakeOnLostTouch = true);
		void RemoveActors(PxActor* const* actors, PxU32 nbActors, bool wakeOnLostTouch = true);

		PxScene* m_pScene;
		PxPhysics* m_pPhysics;
		PxCpuDispatcher* m_pCpuDispatcher;

		std::unordered_map<UID, class PhysicsObject*> m_PhysicsObjectMap;

		//자기자신의 레이어이름, <필터ID, 필터마스크>
		std::unordered_map<std::string, std::pair<physx::PxU32, physx::PxU32>> m_Layer;

		PhysicsInfo m_SceneInfo;

		class BaseEventCallBack* m_EventCallBack;
	};
}

