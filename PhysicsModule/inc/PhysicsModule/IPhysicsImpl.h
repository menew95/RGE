#pragma once

#include "PhysicsModule\PhysicsInfo.h"

namespace Common
{
	class ErrorCallback;
}

namespace PhysicsModule
{
	struct PhysicsInfo;
	struct ColliderInfo;
	struct RigidbodyInfo;
	struct AxisLockInfo;
	class PhysicsObject;
	class PhysicsMaterial;

	class PHYSICS_DLL_DECLSPEC IPhysicsImpl
	{
	public:
		/// <summary>
		/// Create Physics Scene
		/// </summary>
		virtual bool Initalize() abstract;

		/// <summary>
		/// Release All Physics Objects
		/// </summary>
		virtual void Finalize() abstract;
		
		/// <summary>
		/// Setting Physics Scene Setting
		/// </summary>
		/// <param name="">Scene Init Data</param>
		virtual void PhysicsSetting(PhysicsInfo&) abstract;

		/// <summary>
		/// Simulate Physics Scene
		/// </summary>
		/// <param name="deltaTime"></param>
		virtual void Simulate(float dt) abstract;

		/// @brief Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		///	or via a custom user callback implementation inheriting from PxRaycastCallback.
		/// @param origin Origin of the ray
		/// @param unitDir Normalized Direction of the ray
		/// @param reycastHitInfo Raycast hit buffer
		/// @param maxDistance Length of the ray, Has to be in the [0, inf] range
		/// @return If ray hit object in the scene, return object information
		virtual bool Raycast(Math::Vector3 origin, Math::Vector3 unitDir, RaycastHitInfo& reycastHitInfo, float maxDistance, uint32 layerMask) abstract;

		/// @brief Performs a raycast against objects in the scene, returns results in a PxRaycastBuffer object
		///	or via a custom user callback implementation inheriting from PxRaycastCallback.
		/// @param origin Origin of the ray
		/// @param unitDir Normalized Direction of the ray
		/// @param maxDistance Length of the ray, Has to be in the [0, inf] range
		/// @return If ray hit object in the scene, return object information
		virtual bool Raycast(Math::Vector3 origin, Math::Vector3 unitDir, float maxDistance, uint32 layerMask) abstract;

		/// @brief Instance PxMaterial
		/// @param uid Set material uid
		/// @param materialInfo Material information elements data
		/// @return Success
		virtual PhysicsMaterial* CreateMaterial(uuid uid, PhysicsMaterialInfo& materialInfo) abstract;

		/// @brief Create Collider object in current scene
		/// @param userData User data
		/// @param colliderInfo Collider elements information
		/// @param materiaID Material Unique id
		/// @return PhysicsObject pointer
		virtual PhysicsObject* CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, PhysicsMaterial* material = nullptr) abstract;

		/// @brief Create Collider & RigidBody object in current scene
		/// @param userData User data
		/// @param colliderInfo Collider elements information
		/// @param rigidInfo Rigid body elements information
		/// @param materiaID Material unique id
		/// @return PhysicsObject pointer
		virtual PhysicsObject* CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* material = nullptr) abstract;

		/// @brief Delete Collider & RigidBody object in current scene
		/// @param Physics object's pointer
		/// @return Success
		virtual bool DeletePhysicsObject(PhysicsModule::PhysicsObject* physicsObject) abstract;

		virtual std::vector<CollisionEvent>& GetCollisionEvents() abstract;

		virtual std::vector<TriggerEvent>& GetTriggerEvents() abstract;

		virtual void SetFilter(int i, PhysicsLayer& layer) abstract;

		virtual void AddFilter(PhysicsLayer& layer) abstract;

		virtual bool GetRenderBufferPoints(std::vector<DebugPoint>& points) abstract;
		virtual bool GetRenderBufferLines(std::vector<DebugLine>& lines) abstract;
		virtual bool GetRenderBufferTriangles(std::vector<DebugTriangle>& triangles) abstract;

		virtual bool SetErrorCallback(Common::ErrorCallback* errorCallback) abstract;
	};

//#include <memory>
	//extern "C++" PHYSICS_DLL_DECLSPEC std::shared_ptr<IPhysicsImpl> PhysicsImplInstance();

	extern "C"
	{
		PHYSICS_DLL_DECLSPEC IPhysicsImpl* PhysicsImplInstance();
		PHYSICS_DLL_DECLSPEC void PhysicsImplRelease();

		//using PhysicsConstructor = std::shared_ptr<IPhysicsImpl>(*)();
		using PhysicsConstructor = IPhysicsImpl* (*)();
		using PhysicsDestructor = void (*)();
	}
}

using namespace PhysicsModule;