#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\Metal\BaseEventCallBack.h"

PhysicsModule::BaseEventCallBack::~BaseEventCallBack()
{

}

void PhysicsModule::BaseEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

}

void PhysicsModule::BaseEventCallBack::onWake(PxActor** actors, PxU32 count)
{

}

void PhysicsModule::BaseEventCallBack::onSleep(PxActor** actors, PxU32 count)
{

}

void PhysicsModule::BaseEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	auto* actor1= pairHeader.actors[0];
	auto* actor2= pairHeader.actors[1];

	CollisionEvent _collisionEvent;

	_collisionEvent._userData1 = (UserData*)actor1->userData;
	_collisionEvent._userData2 = (UserData*)actor2->userData;

	bool _hasFound = false;
	bool _hasPresists = false;
	bool _hasExit = false;

	
	for (size_t i = 0; i < nbPairs; i++)
	{
		auto _contactPair = &pairs[i];

		if (_contactPair->events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			_hasFound = true;
		}
		else if (_contactPair->events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			_hasPresists = true;
		}
		else if (_contactPair->events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			_hasExit = true;
		}

		PxContactPairPoint _points[10];
		//_points.reserve(_contactPair->contactCount);

		_collisionEvent._contactCount = _contactPair->contactCount;

		_contactPair->extractContacts(_points, _contactPair->contactCount);

		for (int i = 0; i < _contactPair->contactCount; i++)
		{
			ContactPoint _contactPoint;

			_contactPoint._normal = PxVec3ToVector3(_points[i].normal);
			_contactPoint._point = PxVec3ToVector3(_points[i].position);
			_contactPoint._separation = _points[i].separation;

			_collisionEvent._contactPoints.push_back(_contactPoint);
		}
	}


	if (_hasPresists)
	{
		/*  
		* 접점이 유지되고 있기때문에 Stay Event 발생
		*/

		_collisionEvent._eventType = eEventType::Stay;
	}
	else if(_hasFound)
	{
		/* 
		* 이미 접촉 중인 접점이 없기 때문에 Enter 발생
		*/

		_collisionEvent._eventType = eEventType::Enter;
	}
	else
	{
		/*
		* Contact가 새롭게 생성되거나 유지중인 것이 없음으로 모든 접촉이 끝난 상태, Exit 발생
		*/

		_collisionEvent._eventType = eEventType::Exit;
	}
	
	m_CollisionEvents.push_back(_collisionEvent);
}

void PhysicsModule::BaseEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (size_t i = 0; i < count; i++)
	{
		auto* _triggerPair = &pairs[i];

		TriggerEvent _newTriggerEvent;

		_newTriggerEvent._userData1 = reinterpret_cast<UserData*>(_triggerPair->triggerActor->userData);
		_newTriggerEvent._userData2 = reinterpret_cast<UserData*>(_triggerPair->otherActor->userData);

		if (_triggerPair->status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			_newTriggerEvent._eventType = eEventType::Enter;
		}
		else if (_triggerPair->status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			_newTriggerEvent._eventType = eEventType::Exit;
		}
		else
		{
			AssertMessageBox(false, "Trigger event is unknown");
		}

		m_TriggerEvents.push_back(_newTriggerEvent);
	}
}

void PhysicsModule::BaseEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{

}

void PhysicsModule::BaseEventCallBack::Tick()
{
	m_CollisionEvents.clear();
	m_TriggerEvents.clear();
}
