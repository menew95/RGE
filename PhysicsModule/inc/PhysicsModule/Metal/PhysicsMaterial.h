#pragma once

#include "PhysicsModule\PhysicsInfo.h"

namespace physx
{
	class PxMaterial;
}

namespace PhysicsModule
{
	/// @brief PxMaterial wrap class
	class PHYSICS_DLL_DECLSPEC PhysicsMaterial
	{
	public:
		PhysicsMaterial(PhysicsMaterialInfo& materialInfo);
		PhysicsMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo);
		~PhysicsMaterial();

		/// @brief Get PxMaterial pointer
		/// @return PxMaterial pointer
		inline physx::PxMaterial* GetMaterial();

		/// @brief Set uid to PxMaterial userdata
		/// @param uid unique id
		/// @return success
		bool SetUID(uuid uuid);

		/// @brief Set PxMaterial's static friction 
		/// @param friction static friction
		/// @return success
		bool SetStaticFriction(float friction);

		/// @brief Set PxMaterial's dynamic friction
		/// @param friction Dynamic friction
		/// @return success
		bool SetDynamicFriction(float friction);

		/// @brief Set PxMaterial's restitution
		/// @param bounciness Restitution
		/// @return success
		bool SetBounciness(float bounciness);

		/// @brief Set PxMaterial's friction combine mode
		/// @param mode Friction combine mode
		/// @return success
		bool SetFrictionCombineMode(eFrictionCombine mode);

		/// @brief Set PxMaterial's restitution combine mode
		/// @param mode Restitution combine mode
		/// @return success
		bool SetBounceCombineMode(eBounceCombine mode);

	private:
		uuid m_uuid;
		physx::PxMaterial* m_pMaterial;
	};
}

