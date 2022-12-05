#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsFactory.h"
#include "PhysicsModule\PhysicsImpl.h"
#include "PhysicsModule\Metal\PhysicsMaterial.h"
#include "PhysicsModule\PhysicsObject.h"
#include "PhysicsModule\PhysicsScene.h"

using namespace PhysicsModule;

PHYSICS_DLL_DECLSPEC PhysicsScene* PhysicsModule::CreatePhysicsScene()
{
	PhysicsScene* _physicsScene = new PhysicsScene(PhysicsSDK->GetPhysics(), PhysicsSDK->GetCpuDispatcher());

	return _physicsScene;
}

PHYSICS_DLL_DECLSPEC void PhysicsModule::CreatePhysicsMaterial(uuid uuid, PhysicsMaterialInfo& materialInfo)
{
	PhysicsSDK->CreateMaterial(uuid, materialInfo);
}

PHYSICS_DLL_DECLSPEC PhysicsObject* PhysicsModule::CreateCollider(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, PhysicsMaterial* physicsMaterial/*= 0*/)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		return nullptr;
	}
}

PHYSICS_DLL_DECLSPEC PhysicsObject* PhysicsModule::CreateRigidBody(UserData userData, TransformInfo& pose, ColliderInfo& colliderInfo, RigidInfo& rigidInfo, PhysicsMaterial* physicsMaterial/*= 0*/)
{
	PhysicsObject* _physicsObject = new PhysicsObject(userData, pose, colliderInfo, rigidInfo, physicsMaterial);

	if (_physicsObject->IsVaild())
	{
		return _physicsObject;
	}
	else
	{
		return nullptr;
	}
}

PHYSICS_DLL_DECLSPEC bool PhysicsModule::DeletePhysicsObject(PhysicsModule::PhysicsObject* physicsObject)
{
	NullCheck(physicsObject);

	delete physicsObject;

	return true;
}
