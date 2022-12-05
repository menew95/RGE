#pragma once

#include "PhysicsModule\PhysicsInfo.h"

namespace PhysicsModule
{
	/// @brief Create Physics Scene
	PHYSICS_DLL_DECLSPEC class PhysicsScene* CreatePhysicsScene();

	/// @brief Create Physics material
	/// @param Unique id of material
	/// @param Physics material elements information
	PHYSICS_DLL_DECLSPEC void CreatePhysicsMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo);

	/// @brief Create Collider object in current scene
	/// @param userData User data
	/// @param colliderInfo Collider elements information
	/// @param materiaID Material Unique id
	/// @return PhysicsObject pointer
	PHYSICS_DLL_DECLSPEC class PhysicsObject* CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, class PhysicsMaterial* physicsMaterial = nullptr);
	
	/// @brief Create Collider & RigidBody object in current scene
	/// @param userData User data
	/// @param colliderInfo Collider elements information
	/// @param rigidInfo Rigid body elements information
	/// @param materiaID Material unique id
	/// @return PhysicsObject pointer
	PHYSICS_DLL_DECLSPEC class PhysicsObject* CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, class PhysicsMaterial* physicsMaterial = nullptr);

	/// @brief Delete Collider & RigidBody object in current scene
	/// @param Physics object's pointer
	/// @return Success
	PHYSICS_DLL_DECLSPEC bool DeletePhysicsObject(PhysicsModule::PhysicsObject* physicsObject);
}