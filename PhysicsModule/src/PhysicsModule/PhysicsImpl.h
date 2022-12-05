#pragma once
#include <PhysX\PxPhysicsAPI.h>

#include "PhysicsModule\IPhysicsImpl.h"

namespace PhysicsModule
{
	class PhysicsScene;
	class PhysicsMaterial;
	class PhysicsObject;

	class PhysicsImpl :
		public IPhysicsImpl
	{
	public:
		PhysicsImpl();
		~PhysicsImpl();
		

		/// <summary>
		/// Singleton instance
		/// </summary>
		/// <returns></returns>
		static PhysicsImpl* Instance();

		/// <summary>
		/// release singleton instance
		/// </summary>
		static void Release();

		/// <summary>
		/// Create Physics Scene
		/// </summary>
		virtual bool Initalize() override;

		/// <summary>
		/// Release All Physics Objects
		/// </summary>
		virtual void Finalize() override;

		/// <summary>
		/// Setting Physics Scene Setting
		/// </summary>
		/// <param name="">Scene Init Data</param>
		virtual void PhysicsSetting(PhysicsInfo&) override;

		/// <summary>
		/// Simulate Physics Scene
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void Simulate(float dt) override;

		/// @brief Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		///	or via a custom user callback implementation inheriting from PxRaycastCallback.
		/// @param origin Origin of the ray
		/// @param unitDir Normalized Direction of the ray
		/// @param reycastHitInfo Raycast hit buffer
		/// @param maxDistance Length of the ray, Has to be in the [0, inf] range
		/// @return If ray hit object in the scene, return object information
		virtual bool Raycast(Math::Vector3 origin, Math::Vector3 unitDir, RaycastHitInfo& reycastHitInfo, float maxDistance, uint32 layerMask);
		
		/// @brief Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		///	or via a custom user callback implementation inheriting from PxRaycastCallback.
		/// @param origin Origin of the ray
		/// @param unitDir Normalized Direction of the ray
		/// @param maxDistance Length of the ray, Has to be in the [0, inf] range
		/// @return If ray hit object in the scene, return object information
		virtual bool Raycast(Math::Vector3 origin, Math::Vector3 unitDir, float maxDistance, uint32 layerMask);
		
		/// @brief Instance PxMaterial
		/// @param uid Set material uid
		/// @param materialInfo Material information elements data
		/// @return Success
		virtual PhysicsMaterial* CreateMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo) override;

		/// @brief Create Collider object in current scene
		/// @param userData User data
		/// @param colliderInfo Collider elements information
		/// @param materiaID Material Unique id
		/// @return PhysicsObject pointer
		virtual PhysicsObject* CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, PhysicsMaterial* physicsMaterial = nullptr) override;

		/// @brief Create Collider & RigidBody object in current scene
		/// @param userData User data
		/// @param colliderInfo Collider elements information
		/// @param rigidInfo Rigid body elements information
		/// @param materiaID Material unique id
		/// @return PhysicsObject pointer
		virtual PhysicsObject* CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* physicsMaterial = nullptr) override;

		/// @brief Delete Collider & RigidBody object in current scene
		/// @param Physics object's pointer
		/// @return Success
		virtual bool DeletePhysicsObject(PhysicsModule::PhysicsObject* physicsObject) override;

		virtual std::vector<CollisionEvent>& GetCollisionEvents() override;

		virtual std::vector<TriggerEvent>& GetTriggerEvents() override;

		physx::PxPhysics* GetPhysics() { return m_pPhysics; }
		physx::PxFoundation* GetFoundation() { return m_pFoundation; }
		physx::PxCooking* GetCooking() { return m_pCooking; }
		physx::PxPvd* GetPvd() { return m_pPvd; }
		physx::PxDefaultCpuDispatcher* GetCpuDispatcher() { return m_pDefaultCpuDispatcher; }
		
		PhysicsScene* GetScene() { return m_pCurrScene; }
		PhysicsInfo& GetPhysicsInfo() { return m_PhysicsInfo; }

		inline PhysicsMaterial* GetDefaultMaterial() { return m_pDefaultMaterial; }

		virtual void AddFilter(PhysicsLayer& layer);

		virtual void SetFilter(int i, PhysicsLayer& layer);

		virtual PhysicsLayer& GetFilter(int i);

		virtual bool GetRenderBufferPoints(std::vector<DebugPoint>& points);
		virtual bool GetRenderBufferLines(std::vector<DebugLine>& lines);
		virtual bool GetRenderBufferTriangles(std::vector<DebugTriangle>& triangles);

		virtual bool SetErrorCallback(Common::ErrorCallback* errorCallback);
	private:
		bool m_bConnectPvd;

		PhysicsInfo m_PhysicsInfo;

		bool m_IsRecordMemoryAllocations = true;

		physx::PxPhysics* m_pPhysics;
		physx::PxFoundation* m_pFoundation;
		physx::PxCooking* m_pCooking;
		physx::PxPvd* m_pPvd;
		physx::PxPvdTransport* m_pPvdTransport;
		physx::PxDefaultCpuDispatcher* m_pDefaultCpuDispatcher;

		PhysicsMaterial* m_pDefaultMaterial;

		PhysicsScene* m_pCurrScene;

		std::vector<PhysicsLayer> m_PhysicsLayers;
	};

	physx::PxFilterFlags PhysicsWorldFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize
	);

#define PhysicsSDK PhysicsImpl::Instance()
}
