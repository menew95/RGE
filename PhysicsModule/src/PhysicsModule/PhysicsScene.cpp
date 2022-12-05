#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsScene.h"
#include "PhysicsModule\Metal\BaseEventCallBack.h"

#define NullCheckScene NullCheck(m_pScene)

#define SCENE_LOCK() 

PhysicsModule::PhysicsScene::PhysicsScene(physx::PxPhysics* physics, physx::PxCpuDispatcher* dispatcher)
:	m_pPhysics(physics)
,	m_pCpuDispatcher(dispatcher)
,	m_pScene(nullptr)
,	m_EventCallBack(nullptr)
{

}

PhysicsModule::PhysicsScene::~PhysicsScene()
{
	m_pScene->release();
	delete m_EventCallBack;
}

physx::PxFilterFlags PhysicsWorldFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	/*if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}*/
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	{
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
	}
	else
	{
		return PxFilterFlag::eSUPPRESS;
	}

	return PxFilterFlag::eDEFAULT;
}

void PhysicsModule::PhysicsScene::InitScene(PhysicsInfo& sceneInfo)
{
	m_SceneInfo = sceneInfo;

	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(sceneInfo._gravityX, sceneInfo._gravityY, sceneInfo._gravityZ);
	sceneDesc.cpuDispatcher = m_pCpuDispatcher;
	sceneDesc.filterShader = PhysicsWorldFilterShader;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	
	if (sceneInfo._contactsGeneration == eContactsGeneration::PCM)
	{
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	}

	switch (sceneInfo._boardphaseType)
	{
		case eBoardphaseType::SAP:
		{
			sceneDesc.broadPhaseType = PxBroadPhaseType::Enum::eSAP;
			break;
		}
		case eBoardphaseType::MBP:
		{
			sceneDesc.broadPhaseType = PxBroadPhaseType::Enum::eMBP;
			break;
		}
	}

	switch (sceneInfo._frictionType)
	{
		case eFrictionType::PATCH:
		{
			sceneDesc.frictionType = PxFrictionType::Enum::ePATCH;
			break;
		}
		case eFrictionType::ONE_DIRECTIONAL:
		{
			sceneDesc.frictionType = PxFrictionType::Enum::eONE_DIRECTIONAL;
			break;
		}
		case eFrictionType::TWO_DIRECTIONAL:
		{
			sceneDesc.frictionType = PxFrictionType::Enum::eTWO_DIRECTIONAL;
			break;
		}
	}

	sceneDesc.bounceThresholdVelocity = sceneInfo._bounceThreshold;

	PxVec3 minBound(sceneInfo._worldBoundsCenterX - sceneInfo._worldBoundsExtentX
		, sceneInfo._worldBoundsCenterX - sceneInfo._worldBoundsExtentY
		, sceneInfo._worldBoundsCenterX - sceneInfo._worldBoundsExtentZ);
	PxVec3 maxBound(sceneInfo._worldBoundsCenterX + sceneInfo._worldBoundsExtentX
		, sceneInfo._worldBoundsCenterX + sceneInfo._worldBoundsExtentY
		, sceneInfo._worldBoundsCenterX + sceneInfo._worldBoundsExtentZ);
	
	sceneDesc.sanityBounds.minimum = minBound;
	sceneDesc.sanityBounds.maximum = maxBound;

	m_EventCallBack = new BaseEventCallBack();
	sceneDesc.simulationEventCallback = m_EventCallBack;

	//sceneDesc.contactModifyCallback
	//sceneDesc.ccdContactModifyCallback
	//sceneDesc.filterShaderData
	//sceneDesc.filterShaderDataSize
	//sceneDesc.filterShader
	//sceneDesc.filterCallback
	//sceneDesc.kineKineFilteringMode
	//sceneDesc.staticKineFilteringMode
	//sceneDesc.broadPhaseCallback
	//sceneDesc.limits
	//sceneDesc.solverType
	//sceneDesc.frictionOffsetThreshold
	//sceneDesc.ccdMaxSeparation
	//sceneDesc.solverOffsetSlop
	//sceneDesc.cudaContextManager
	//sceneDesc.staticStructure
	//sceneDesc.dynamicStructure
	//sceneDesc.sceneQueryUpdateMode
	//sceneDesc.userData
	//sceneDesc.solverBatchSize
	//sceneDesc.solverArticulationBatchSize
	//sceneDesc.nbContactDataBlocks
	//sceneDesc.maxNbContactDataBlocks
	//sceneDesc.maxBiasCoefficient
	//sceneDesc.contactReportStreamBufferSize
	//sceneDesc.ccdMaxPasses
	//sceneDesc.ccdThreshold
	//sceneDesc.wakeCounterResetValue
	//sceneDesc.sanityBounds
	//sceneDesc.gpuDynamicsConfig
	//sceneDesc.gpuMaxNumPartitions
	//sceneDesc.gpuComputeVersion

	m_pScene = m_pPhysics->createScene(sceneDesc);
	
#if defined(DEBUG) || defined(_DEBUG)
	//_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	//_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eWORLD_AXES, true);
	//_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eBODY_AXES, true);

	m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 2.0f);
	physx::PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
	if (pvdClient)
	{
		PxPvdSceneFlags flags = PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS | PxPvdSceneFlag::eTRANSMIT_CONTACTS
			| PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES;
		pvdClient->setScenePvdFlags(flags);
	}

#endif
}

UID PhysicsModule::PhysicsScene::CreatePhysicObject()
{
	UID uid = 0;

	return uid;
}

bool PhysicsModule::PhysicsScene::DeletePhysicsObject(UID id)
{


	return true;
}

void PhysicsModule::PhysicsScene::AddActors(PxActor* const* actors, PxU32 nbActors)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->addActors(actors, nbActors);
}

void PhysicsModule::PhysicsScene::AddActors(const PxPruningStructure& pruningStructure)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->addActors(pruningStructure);
}

void PhysicsModule::PhysicsScene::RemoveActor(PxActor& actor, bool wakeOnLostTouch /*= true*/)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->removeActor(actor, wakeOnLostTouch);
}

void PhysicsModule::PhysicsScene::RemoveActors(PxActor* const* actors, PxU32 nbActors, bool wakeOnLostTouch /*= true*/)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->removeActors(actors, nbActors, wakeOnLostTouch);
}

void PhysicsModule::PhysicsScene::AddAggregate(PxAggregate& aggregate)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->addAggregate(aggregate);
}

void PhysicsModule::PhysicsScene::RemoveAggregate(PxAggregate& aggregate, bool wakeOnLostTouch /*= true*/)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->removeAggregate(aggregate, wakeOnLostTouch);
}

void PhysicsModule::PhysicsScene::AddCollection(const PxCollection& collection)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->addCollection(collection);
}

void PhysicsModule::PhysicsScene::ShiftOrigin(const PxVec3& shift)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_pScene->shiftOrigin(shift);
}

physx::PxPvdSceneClient* PhysicsModule::PhysicsScene::GetScenePvdClient()
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	return m_pScene->getScenePvdClient();
}

bool PhysicsModule::PhysicsScene::Raycast(PxVec3 origin, PxVec3 unitDir, PxReal maxDistance, PxRaycastBuffer& buffer, uint32 layerMask)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	PxHitFlags flags = PxHitFlag::eDEFAULT | PxHitFlag::eUV | PxHitFlag::eMESH_ANY;

	PxQueryFilterData _queryFilterData;

	_queryFilterData.data = PxFilterData(layerMask, 0, 0, 0);
	_queryFilterData.flags = PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC;

	return m_pScene->raycast(origin, unitDir, maxDistance, buffer, flags, _queryFilterData);
}

void PhysicsModule::PhysicsScene::Simulate(float elapseTime)
{
#ifdef _DEBUG
	NullCheckScene;
#endif // _DEBUG

	m_EventCallBack->Tick();

	m_pScene->simulate(elapseTime);
	m_pScene->fetchResults(true);
}

std::vector<PhysicsModule::CollisionEvent>& PhysicsModule::PhysicsScene::GetCollisionEvents()
{
	return m_EventCallBack->GetCollisionEvents();
}

std::vector<PhysicsModule::TriggerEvent>& PhysicsModule::PhysicsScene::GetTriggerEvents()
{
	return m_EventCallBack->GetTriggerEvents();
}

bool PhysicsModule::PhysicsScene::SetEventCallback(PxSimulationEventCallback* eventCallback)
{
#ifdef _DEBUG
	NullCheckScene;
#endif

	m_pScene->setSimulationEventCallback(eventCallback);

	return true;
}
