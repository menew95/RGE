#pragma once

namespace PhysicsModule
{
	class RigidActor;
	class Shape;
	class PhysicsMaterial;
	/// <summary>
	/// Physics Object Interface
	/// </summary>
	class PHYSICS_DLL_DECLSPEC PhysicsObject
	{
	public:
		PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, PhysicsMaterial* material = nullptr);
		PhysicsObject(UserData& userData, TransformInfo& transformInfo, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* material = nullptr);
		~PhysicsObject();

		//////////////////////////////////////////////////////////////////////////
		// Create collider & rigidbody
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Create shape
		/// </summary>
		/// <param name="colliderInfo">collider elements</param>
		/// <returns>creation success</returns>
		bool CreateShape(ColliderInfo& colliderInfo, PhysicsMaterial* material = nullptr);

		/// <summary>
		/// Create rigid actor
		/// </summary>
		/// <param name="rigidInfo">rigidbody elements</param>
		/// <returns>creation success</returns>
		bool CreateRigidActor(UserData& userData, TransformInfo& transformInfo, RigidInfo& rigidInfo);

		/// @brief Create static rigid actor
		/// @param userData game object information
		/// @param transformInfo transform information
		/// @return success
		bool CreateColliderActor(UserData& userData, TransformInfo& transformInfo);

		bool OnEnable();
		bool OnDisable();

		//////////////////////////////////////////////////////////////////////////
		// Transform Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set rigid actor's global position & rotation
		/// </summary>
		/// <param name="transformInfo">global position & rotation</param>
		void SetTransform(TransformInfo & transformInfo);

		/// <summary>
		/// Get rigid actor's global position & rotation
		/// </summary>
		/// <returns>global position & rotation</returns>
		TransformInfo GetTransform();

		//////////////////////////////////////////////////////////////////////////
		// Collider Control
		//////////////////////////////////////////////////////////////////////////

		bool SetLayer(int i);

		/// <summary>
		/// Set shape trigger flag
		/// </summary>
		/// <param name="val">flag</param>
		void SetIsTrigger(bool val);

		/// <summary>
		/// Set box & sphere & capsule center
		/// </summary>
		/// <param name="center">center offset</param>
		void SetCenter(Math::Vector3 center);

		/// <summary>
		/// Set box half-size
		/// </summary>
		/// <param name="size">half-size</param>
		void SetSize(Math::Vector3 size);

		/// <summary>
		/// Set capsule & sphere radius
		/// </summary>
		/// <param name="size">radius</param>
		void SetRadius(float radius);
		
		/// <summary>
		/// Set capsule height
		/// </summary>
		/// <param name="size">capsule height</param>
		void SetHeight(float height);

		/// <summary>
		/// Set capsule Axis-direction
		/// </summary>
		/// <param name="dir">Axis-direction</param>
		void SetAxisDirection(eAxisDirection dir);

		//////////////////////////////////////////////////////////////////////////
		// RigidBody Control
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Set rigid body's mass value
		/// </summary>
		/// <param name="mass">mass</param>
		/// <returns>success</returns>
		bool SetMass(float mass);

		/// <summary>
		/// Set rigid body's linear drag
		/// </summary>
		/// <param name="damping">damping</param>
		/// <returns>success</returns>
		bool SetLinearDamping(float damping);

		/// <summary>
		/// Set rigid body's angular drag
		/// </summary>
		/// <param name="damping">angular damping</param>
		/// <returns>success</returns>
		bool SetAngularDamping(float damping);

		/// <summary>
		/// Set rigid body's gravify flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		bool SetGravity(bool flag);

		/// <summary>
		/// Set rigid body's kinematic flag
		/// </summary>
		/// <param name="flag">flag</param>
		/// <returns>success</returns>
		bool SetKinematic(bool flag);

		/// <summary>
		/// Set rigid body's axis lock flag
		/// </summary>
		/// <param name="constraints">constraints</param>
		/// <returns>success</returns>
		bool SetConstraints(Constraints& constraints);

		bool SetCollisionDetection(eCollisionDetection& collisionDetection);

		bool SetLinearVelocity(Math::Vector3 velocity);
		bool SetAngularVelocity(Math::Vector3 velocity);
		bool SetMaxLinearVelocity(float val);
		bool SetMaxAngularVelocity(float val);

		//////////////////////////////////////////////////////////////////////////
		// External force
		//////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Applying force to a body
		/// </summary>
		/// <param name="vector">linear force</param>
		/// <returns>success</returns>
		bool AddForce(Math::Vector3 vector, int forceMode);

		/// <summary>
		/// Apply a torque to a rigid body
		/// </summary>
		/// <param name="torque">torque</param>
		/// <returns>success</returns>
		bool AddTorque(Math::Vector3 torque, int forceMode);

		//////////////////////////////////////////////////////////////////////////
		// ETC
		//////////////////////////////////////////////////////////////////////////

		/// @brief This physics object is vaild
		/// @return vaild
		bool IsVaild() { return m_IsVaild; }

	private:
		RigidActor* m_pRigidActor;
		Shape* m_pShape;

		ColliderInfo& m_ColliderInfoRef;
		RigidInfo m_RigidInfo;

		bool m_IsVaild = false;
	};
}