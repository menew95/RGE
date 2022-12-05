#pragma once

#include "PhysicsModule\PhysicsInfo.h"

namespace PhysicsModule
{
	class RigidActor;
	class Shape;

	/// <summary>
	/// Physics Object Interface
	/// </summary>
	class PHYSICS_DLL_DECLSPEC IPhysicsObject
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		// Create collider & rigidbody
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Create shape
		/// </summary>
		/// <param name="colliderInfo">collider elements</param>
		/// <returns>creation success</returns>
		virtual bool CreateShape(ColliderInfo& colliderInfo, UID uid = 0) abstract;

		/// <summary>
		/// Create rigid actor
		/// </summary>
		/// <param name="rigidInfo">rigidbody elements</param>
		/// <returns>creation success</returns>
		virtual bool CreateRigidActor(UserData& userData, TransformInfo& transformInfo, RigidInfo& rigidInfo) abstract;

		/// @brief Create static rigid actor
		/// @param userData game object information
		/// @param transformInfo transform information
		/// @return success
		virtual bool CreateColliderActor(UserData& userData, TransformInfo& transformInfo) abstract;

		virtual bool OnEnable() abstract;
		virtual bool OnDisable() abstract;

		//////////////////////////////////////////////////////////////////////////
		// Transform Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set rigid actor's global position & rotation
		/// </summary>
		/// <param name="transformInfo">global position & rotation</param>
		virtual void SetTransform(TransformInfo& transformInfo) abstract;

		/// <summary>
		/// Get rigid actor's global position & rotation
		/// </summary>
		/// <returns>global position & rotation</returns>
		virtual TransformInfo GetTransform() abstract;

		//////////////////////////////////////////////////////////////////////////
		// Collider Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set shape trigger flag
		/// </summary>
		/// <param name="val">flag</param>
		virtual void SetIsTrigger(bool val) abstract;

		/// <summary>
		/// Set box & sphere & capsule center
		/// </summary>
		/// <param name="center">center offset</param>
		virtual void SetCenter(Math::Vector3 center) abstract;

		/// <summary>
		/// Set box half-size
		/// </summary>
		/// <param name="size">half-size</param>
		virtual void SetSize(Math::Vector3 size) abstract;

		/// <summary>
		/// Set capsule & sphere radius
		/// </summary>
		/// <param name="size">radius</param>
		virtual void SetRadius(float radius) abstract;

		/// <summary>
		/// Set capsule height
		/// </summary>
		/// <param name="size">capsule height</param>
		virtual void SetHeight(float height) abstract;

		/// <summary>
		/// Set capsule Axis-direction
		/// </summary>
		/// <param name="dir">Axis-direction</param>
		virtual void SetAxisDirection(eAxisDirection dir) abstract;

		//////////////////////////////////////////////////////////////////////////
		// RigidBody Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set rigid body's mass value
		/// </summary>
		/// <param name="mass">mass</param>
		/// <returns>success</returns>
		virtual bool SetMass(float mass) abstract;

		/// <summary>
		/// Set rigid body's linear drag
		/// </summary>
		/// <param name="damping">damping</param>
		/// <returns>success</returns>
		virtual bool SetLinearDamping(float damping) abstract;

		/// <summary>
		/// Set rigid body's angular drag
		/// </summary>
		/// <param name="damping">angular damping</param>
		/// <returns>success</returns>
		virtual bool SetAngularDamping(float damping) abstract;

		/// <summary>
		/// Set rigid body's gravity flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		virtual bool SetGravity(bool flag) abstract;

		/// <summary>
		/// Set rigid body's kinematic flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		virtual bool SetKinematic(bool flag) abstract;

		/// <summary>
		/// Set rigid body's axis lock flag
		/// </summary>
		/// <param name="constraints">constraints</param>
		/// <returns>success</returns>
		virtual bool SetConstraints(Constraints& constraints) abstract;

		virtual bool SetCollisionDetection(eCollisionDetection& collisionDetection) abstract;

		virtual bool SetLinearVelocity(Math::Vector3 velocity) abstract;
		virtual bool SetAngularVelocity(Math::Vector3 velocity) abstract;
		virtual bool SetMaxLinearVelocity(float val) abstract;
		virtual bool SetMaxAngularVelocity(float val) abstract;

		//////////////////////////////////////////////////////////////////////////
		// External force
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Applying force to a body
		/// </summary>
		/// <param name="vector">linear force</param>
		/// <returns>success</returns>
		virtual bool AddForce(Math::Vector3 vector, int forceMode) abstract;

		/// <summary>
		/// Apply a torque to a rigid body
		/// </summary>
		/// <param name="torque">torque</param>
		/// <returns>success</returns>
		virtual bool AddTorque(Math::Vector3 torque, int forceMode) abstract;

		//////////////////////////////////////////////////////////////////////////
		// ETC
		//////////////////////////////////////////////////////////////////////////
	};
}