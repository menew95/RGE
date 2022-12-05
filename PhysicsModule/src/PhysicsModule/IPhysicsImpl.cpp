#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\IPhysicsImpl.h"
#include "PhysicsModule\PhysicsImpl.h"

//static std::shared_ptr<IPhysicsImpl> g_physicsImpl = nullptr;
static PhysicsModule::IPhysicsImpl* g_physicsImpl = nullptr;

//PHYSICS_DLL_DECLSPEC std::shared_ptr<IPhysicsImpl> Physics::PhysicsImplInstance()
//{
//	if (g_physicsImpl == nullptr)
//	{
//		g_physicsImpl = std::make_shared<Physics::PhysicsImpl>();
//		g_physicsImpl->Initalize();
//	}
//
//	return g_physicsImpl;
//}

PHYSICS_DLL_DECLSPEC IPhysicsImpl* PhysicsModule::PhysicsImplInstance()
{
	if (g_physicsImpl == nullptr)
	{
		g_physicsImpl = PhysicsImpl::Instance();
	}

	return g_physicsImpl;
}

PHYSICS_DLL_DECLSPEC void PhysicsModule::PhysicsImplRelease()
{
	g_physicsImpl->Finalize();

	delete g_physicsImpl;

	g_physicsImpl = nullptr;
}
