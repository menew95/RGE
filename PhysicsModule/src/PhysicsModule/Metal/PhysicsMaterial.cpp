#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsImpl.h"
#include "PhysicsModule\Metal\PhysicsMaterial.h"

using namespace physx;

PhysicsModule::PhysicsMaterial::PhysicsMaterial(PhysicsMaterialInfo& materialInfo)
{
	m_pMaterial = PhysicsSDK->GetPhysics()->createMaterial(materialInfo._staticFriction,
		materialInfo._dynamicFriction,
		materialInfo._bounciness);

	AssertMessageBox(m_pMaterial != nullptr, "Failed to instance PxMaterial");

	m_pMaterial->setFrictionCombineMode((PxCombineMode::Enum)materialInfo._frictionCombine);
	m_pMaterial->setRestitutionCombineMode((PxCombineMode::Enum)materialInfo._bounceCombine);
}

PhysicsModule::PhysicsMaterial::PhysicsMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo)
	: m_uuid(uuid)
{
	m_pMaterial = PhysicsSDK->GetPhysics()->createMaterial(materialInfo._staticFriction,
		materialInfo._dynamicFriction,
		materialInfo._bounciness);

	AssertMessageBox(m_pMaterial != nullptr, "Failed to instance PxMaterial");

	m_pMaterial->setFrictionCombineMode((PxCombineMode::Enum)materialInfo._frictionCombine);
	m_pMaterial->setRestitutionCombineMode((PxCombineMode::Enum)materialInfo._bounceCombine);

	m_pMaterial->userData = &m_uuid;
}

PhysicsModule::PhysicsMaterial::~PhysicsMaterial()
{
	SAFE_RELEASE(m_pMaterial);
}

physx::PxMaterial* PhysicsModule::PhysicsMaterial::GetMaterial()
{
	return m_pMaterial;
}

bool PhysicsModule::PhysicsMaterial::SetUID(uuid uuid)
{
	NullCheck(m_pMaterial);

	m_uuid = uuid;

	return true;
}

bool PhysicsModule::PhysicsMaterial::SetStaticFriction(float friction)
{
	NullCheck(m_pMaterial);

	m_pMaterial->setStaticFriction(friction);
	return true;
}

bool PhysicsModule::PhysicsMaterial::SetDynamicFriction(float friction)
{
	NullCheck(m_pMaterial);

	m_pMaterial->setDynamicFriction(friction);

	return true;
}

bool PhysicsModule::PhysicsMaterial::SetBounciness(float bounciness)
{
	NullCheck(m_pMaterial);

	m_pMaterial->setRestitution(bounciness);

	return true;
}

bool PhysicsModule::PhysicsMaterial::SetFrictionCombineMode(eFrictionCombine mode)
{
	NullCheck(m_pMaterial);

	m_pMaterial->setFrictionCombineMode((PxCombineMode::Enum)mode);

	return true;
}

bool PhysicsModule::PhysicsMaterial::SetBounceCombineMode(eBounceCombine mode)
{
	NullCheck(m_pMaterial);

	m_pMaterial->setRestitutionCombineMode((PxCombineMode::Enum)mode);

	return true;
}
