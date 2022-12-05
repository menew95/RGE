#pragma once

#include <PhysX\PxPhysicsAPI.h>

#include "PhysicsModule\PhysicsInfo.h"

using namespace physx;

namespace PhysicsModule
{
	class Shape;

	enum class eRigidType
	{
		Static
		, Dynamic
	};

	class RigidActor
	{
	public:
		RigidActor(UserData userData, RigidInfo& rigidInfo, physx::PxRigidActor* rigidActor);
		RigidActor(UserData userData, TransformInfo& transformInfo, physx::PxShape* shape);
		RigidActor(UserData userData, TransformInfo& transformInfo, physx::PxShape* shape, RigidInfo& rigidInfo);
		~RigidActor();

		/// @brief Release actor
		void Release();

		/// @brief Instance PxRigidStatic type actor
		/// @param transform Transform information
		/// @param shape Shape pointer attach to actor
		void CreateStatic(PxTransform& transform, physx::PxShape* shape);
		
		/// @brief Instance PxRigidDynamic type actor
		/// @param rigidInfo Rigid body information
		/// @param transform Transform information
		/// @param shape Shape pointer attach to actor
		void CreateDynamic(RigidInfo& rigidInfo, PxTransform& transform, physx::PxShape* shape);

		/// @brief Setting rigid body's flags & elements
		/// @param rigidInfo Rigid body's data
		/// @return Success
		bool InitRigidBody(RigidInfo& rigidInfo);

		/// @brief Change dynamic actor to static actor
		/// @param transformInfo Transform information
		/// @param shape Shape pointer
		/// @return Success
		bool ChangeToCollider(TransformInfo& transformInfo, PxShape* shape);

		/// @brief Change static actor to dynamic actor
		/// @param transformInfo Transform information
		/// @param shape Shape pointer
		/// @param rigidInfo Rigid body information
		/// @return Success
		bool ChangeToRigidBody(TransformInfo& transformInfo, PxShape* shape, RigidInfo& rigidInfo);

		/// @brief If this rigid actor type is dynamic then return true;
		/// @return Actor type is dynamic
		bool IsDynamic();

		/// @brief Add linear force to PxRigidBody
		/// @param vector Linear force
		/// @return Success
		bool AddForce(Math::Vector3 vector, int forceMode);

		/// @brief Add angular force to PxRigidBody
		/// @param torque Angular force
		/// @return Success
		bool AddTorque(Math::Vector3 torque, int forceMode);
		
		/// <summary>
		///  Set actor's world transform before simulate
		/// </summary>
		/// <param name="transform"></param>
		/// <returns></returns>
		bool SetTransform(TransformInfo& transform);

		/// @brief Get actor's world transform after simulate
		/// @return Transform information
		TransformInfo GetTransform();

		/// <summary>
		/// Get collider is trigger
		/// </summary>
		/// <param name="flag"></param>
		/// <returns></returns>
		bool SetIsTrigger(bool flag);

		/// Control Physics Object Settings
		bool SetLinearDamping(float damping);
		bool SetAngularDamping(float damping);
		bool SetLinearVelocity(Math::Vector3 velocity);
		bool SetAngularVelocity(Math::Vector3 velocity);
		bool SetMaxLinearVelocity(float vel);
		bool SetMaxAngularVelocity(float vel);
		bool SetGravity(bool flag);
		bool SetKinematic(bool flag);
		bool SetMass(float mass);
		bool SetConstraints(Constraints& constraints);

		bool SetCollisionDetection(eCollisionDetection collisionDetection);

		bool OnEnable();
		bool OnDisable();

		// don't used
		/*float GetLinearDamping();
		float GetAngularDamping();
		TL_Math::Vector3& GetLineraVelocity();
		TL_Math::Vector3& GetAngularVelocity();
		float GetMaxLinearVelocity();
		float GetMaxAngularVelocity();
		bool GetGravity();
		bool GetKinematic();
		bool GetMass();*/
		PxRigidActor* GetActor() { return m_pRigidActor; }
	private:
		PxRigidActor* m_pRigidActor;
		RigidInfo m_RigidInfo;
		eRigidType m_eRigidType;

		UserData m_UserData;
	};
}
