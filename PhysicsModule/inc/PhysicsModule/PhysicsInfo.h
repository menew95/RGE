#pragma once

#include <string>

#include "Math\Math.h"

#ifdef PHYSICS_EXPORTING
	#include "Common.h"
	#define PHYSICS_DLL_DECLSPEC __declspec(dllexport)//COMMON_API
	#define PHYSICS_DLL_STRUCT __declspec(dllexport)//COMMON_API
#else
	#include "Common.h"
	#define PHYSICS_DLL_DECLSPEC __declspec(dllimport)//COMMON_API
	#define PHYSICS_DLL_STRUCT
#endif

struct PHYSICS_DLL_STRUCT Math::Vector3;
struct PHYSICS_DLL_STRUCT Math::Vector4;

namespace PhysicsModule
{
	/**
	 * @brief Physics object's transform data
	*/
	struct PHYSICS_DLL_STRUCT TransformInfo
	{
		Math::Vector3 _pos;
		Math::Vector4 _rot;
	};

	/**
	 * @brief How the friction of two colliding objects is combined.
	*/
	enum class eFrictionCombine
	{
		Average = 0
		, Minimum = 1
		, Maximum = 3
		, Multyply = 2
	};

	/**
	 * @brief How the bounciness of two colliding objects is combined. It has the same modes as Friction Combine Mode
	*/
	enum class eBounceCombine
	{
		Average = 0
		, Minimum = 1
		, Maximum = 3
		, Multyply = 2
	};

	struct PHYSICS_DLL_STRUCT PhysicsMaterialInfo
	{
		PhysicsMaterialInfo()
		: _dynamicFriction(0.6f) 
		, _staticFriction(0.6f)
		, _bounciness(0.1f)
		, _frictionCombine(eFrictionCombine::Average)
		, _bounceCombine(eBounceCombine::Average)
		{}

		PhysicsMaterialInfo(float dynamicFriction, float staticFriction, float bounciness
		, eFrictionCombine frictionCombine, eBounceCombine bounceCombine)
			: _dynamicFriction(dynamicFriction)
			, _staticFriction(staticFriction)
			, _bounciness(bounciness)
			, _frictionCombine(frictionCombine)
			, _bounceCombine(bounceCombine)
		{}

		float _dynamicFriction;				// [0, inf]
		float _staticFriction;				// [0, inf]
		float _bounciness;					// [0, 1]
		eFrictionCombine _frictionCombine;
		eBounceCombine _bounceCombine;
	};

	/**
	 * @brief Contact generation method.
	*/
	enum class eContactsGeneration
	{
		SAT = 0
		, PCM = 1
	};

	/**
	 * @brief The type of contact pair generation to use.
	*/
	enum class eContactPairsMode
	{
		DefaultContactPairs = 0
		, EnableKinematicKinematicPairs = 1
		, EnableKinematicStaticPairs = 2
		, EnableAllContactPairs = 3
	};

	/**
	 * @brief Broad-phase algorithm to use in the physics simulation.
	*/
	enum class eBoardphaseType
	{
		SAP = 0
		, MBP = 1
	};

	/**
	 * @brief The friction algorithm used for simulation.
	*/
	enum class eFrictionType
	{
		PATCH
		, ONE_DIRECTIONAL
		, TWO_DIRECTIONAL
		//,	FRICTION_COUNT
	};

	/**
	 * @brief The PhysX solver type to use for the simulation.
	*/
	enum class eSolverType
	{
		ProjectedGaussSeidel = 0
		, TemporalGaussSeidel = 1
	};

	/**
	 * @brief Physics scene's init setting elements
	*/
	struct PHYSICS_DLL_STRUCT PhysicsInfo
	{
		PhysicsInfo()
		: _gravityX(0.0f)
		, _gravityY(-9.8f)
		, _gravityZ(0.0f)
		, _physicsMaterial()
		, _bounceThreshold(2.0f)
		, _defaultMaxDepenetreationVelocity(10.0f)
		, _sleepThreshold(0.005f)
		, _defaultContactOffset(0.01f)
		, _defaultSolverIterations(6)
		, _queriesHitBackfaces(1)
		, _queriesHitTiggers(false)
		, _enableAdaptiveForce(true)
		, _contactsGeneration(eContactsGeneration::PCM)
		, _autoSimulation(true)
		, _autoSyncTransforms(false)
		, _contackPairsMode(eContactPairsMode::DefaultContactPairs)
		, _boardphaseType(eBoardphaseType::SAP)
		, _worldBoundsCenterX(0)
		, _worldBoundsCenterY(0)
		, _worldBoundsCenterZ(0)
		, _worldBoundsExtentX(250)
		, _worldBoundsExtentY(250)
		, _worldBoundsExtentZ(250)
		, _worldSubdivisions(6)
		, _frictionType(eFrictionType::PATCH)
		, _enableEnhancedDeterminism(false)
		, _enableUnifiedHeightmaps(true)
		, _solverType(eSolverType::ProjectedGaussSeidel)
		{}
		// Gravity
		float _gravityX;
		float _gravityY;
		float _gravityZ;

		// If do not setting physics material then this default material is used
		PhysicsMaterialInfo _physicsMaterial;

		float _bounceThreshold;
		float _defaultMaxDepenetreationVelocity;
		float _sleepThreshold;

		// Joint 
		float _defaultContactOffset;
		float _defaultSolverIterations;

		bool _queriesHitBackfaces;
		bool _queriesHitTiggers;
		bool _enableAdaptiveForce;

		eContactsGeneration _contactsGeneration;
		bool _autoSimulation;
		bool _autoSyncTransforms;
		eContactPairsMode _contackPairsMode;

		eBoardphaseType _boardphaseType;

		float _worldBoundsCenterX;
		float _worldBoundsCenterY;
		float _worldBoundsCenterZ;

		float _worldBoundsExtentX;
		float _worldBoundsExtentY;
		float _worldBoundsExtentZ;

		float _worldSubdivisions;

		eFrictionType _frictionType;

		bool _enableEnhancedDeterminism;
		bool _enableUnifiedHeightmaps;

		eSolverType _solverType;
	};

	/**
	 * @brief Try one of the options only if you are seeing jerkiness in your Rigid body¡¯s movement.
	*/
	enum class eInterpolate
	{
		None
		, Interpolate
		, Extrapolate
	};

	/**
	 * @brief Prevents fast-moving objects from passing by other objects without detecting a collision.
	*/
	enum class eCollisionDetection
	{
		Discrete
		, Continuous
		, ContinuousSpeculative
	};

	/**
	 * @brief Constraints Axis
	*/
	struct PHYSICS_DLL_STRUCT Freeze
	{
		Freeze()
		: _x(false)
		, _y(false)
		, _z(false)
		{}
		bool _x;
		bool _y;
		bool _z;
	};

	/**
	 * @brief Constraints on rigid bodies
	*/
	struct PHYSICS_DLL_STRUCT Constraints
	{
		Constraints()
		: _position()
		, _rotation()
		{}
		Freeze _position;
		Freeze _rotation;
	};

	/**
	 * @brief Rigid Body Elements
	*/
	struct PHYSICS_DLL_STRUCT RigidInfo
	{
		RigidInfo()
		: _mass(1.0f)
		, _drag(0.05f)
		, _angularDrag(0.05f)
		, _useGravity(true)
		, _isKinematic(false)
		, _interpolate(eInterpolate::None)
		, _collisionDetection(eCollisionDetection::Discrete)
		, _constraints()
		{}
		float _mass;
		float _drag;
		float _angularDrag;
		bool _useGravity;
		bool _isKinematic;

		eInterpolate _interpolate;
		eCollisionDetection _collisionDetection;
		Constraints _constraints;
	};

	/**
	 * @brief Collider Shape Type
	*/
	enum class eShapeType
	{
		BOX
		, SPHERE
		, CAPSULE
		, MESH
		, HEIGHTFIELD
	};

	/**
	 * @brief Collider Elements
	*/
	struct PHYSICS_DLL_STRUCT ColliderInfo
	{
		ColliderInfo(eShapeType type)
		: _shapeType(type)
		, _isTrigger(false)
		, _material(nullptr)
		{}

		eShapeType _shapeType;
		bool _isTrigger;
		PhysicsMaterialInfo* _material;
	};

	/**
	 * @brief Box Collider Elements
	*/
	struct BoxColliderInfo : ColliderInfo
	{
		BoxColliderInfo()
		: ColliderInfo(eShapeType::BOX)
		, _center{ 0.f, 0.f, 0.f }
		, _size{ 1.f, 1.f, 1.f }
		{}

		Math::Vector3 _center;
		Math::Vector3 _size;
	};

	/**
	 * @brief Sphere Collider Elements
	*/
	struct SphereColliderInfo : ColliderInfo
	{
		SphereColliderInfo()
		: ColliderInfo(eShapeType::SPHERE)
		, _center{ 0.f, 0.f, 0.f }
		, _radius(0.5f)
		{}

		Math::Vector3 _center;
		float _radius;
	};

	/**
	 * @brief Capsule Collider Axis Direction
	*/
	enum class eAxisDirection
	{
		X_Axis
		, Y_Axis
		, Z_Axis
	};

	/**
	 * @brief Capsule Collider Elements
	*/
	struct CapsuleColliderInfo : ColliderInfo
	{
		CapsuleColliderInfo()
		: ColliderInfo(eShapeType::CAPSULE)
		, _center{ 0.f, 0.f, 0.f }
		, _radius(0.5f)
		, _height(1.0f)
		, _axisDirection(eAxisDirection::Y_Axis)
		{}

		Math::Vector3 _center;
		float _radius;
		float _height;
		eAxisDirection _axisDirection;
	};

	/**
	 * @brief MeshColldier Elements
	*/
	struct MeshColliderInfo : ColliderInfo
	{
		MeshColliderInfo()
		: ColliderInfo(eShapeType::MESH)
		, _convex(false)
		, _meshData(nullptr)
		{}

		bool _convex = false;
		const void* _meshData = nullptr;
		uint32 _stride = 0;
		uint32 _count = 0;

		uint32* _triData =  nullptr;
		uint32 _triStride = 0;
		uint32 _triCount = 0;
	};
	
	struct HeightFieldColliderInfo : ColliderInfo
	{
		HeightFieldColliderInfo()
			: ColliderInfo(eShapeType::HEIGHTFIELD)
			, _heightFieldData(nullptr)
			, _column(0)
			, _row(0)
		{}

		const void* _heightFieldData;
		uint32 _column;
		uint32 _row;

	};

	/**
	 * @brief user can assign this to whatever, usually to create a 1:1 relationship with a user object.
	*/
	struct PHYSICS_DLL_STRUCT UserData
	{
		UserData(const tchar* name, UID uid, void* object, void* rigidbody, void* collider)
		: _name(name)
		, _uid(uid)
		, _pObject(object)
		, _pRigidBody(rigidbody)
		, _pCollider(collider)
		{}

		tstring _name;
		UID _uid;
		void* _pObject;
		void* _pRigidBody;
		void* _pCollider;
	};

	/// @brief Stores results of raycast queries.
	struct PHYSICS_DLL_STRUCT RaycastHitInfo
	{
		RaycastHitInfo()
		: _userData(nullptr)
		, _distance(0.f)
		, _normal{ 0.f, 0.f, 0.f }
		, _point{ 0.f, 0.f, 0.f }
		, _u(0.f)
		, _v(0.f)
		, _faceIndex(0)
		{}

		RaycastHitInfo(UserData* userData, float dis, Math::Vector3 n, Math::Vector3 p, float u, float v, uint64 faceIndex)
		: _userData(userData)
		, _distance(dis)
		, _normal(n)
		, _point(p)
		, _u(u)
		, _v(v)
		, _faceIndex(faceIndex)
		{

		}

		UserData* _userData;
		float _distance;
		Math::Vector3 _normal;
		Math::Vector3 _point;
		float _u;
		float _v;
		uint64 _faceIndex;
	};

	struct PHYSICS_DLL_STRUCT ContactPoint
	{
		/// @brief Normal of the contact point.
		Math::Vector3 _normal;

		/// @brief 	The point of contact.
		Math::Vector3 _point;

		/// @brief 	The distance between the colliders at the contact point.
		float _separation;
	};

	template class PHYSICS_DLL_STRUCT std::vector<ContactPoint>;

	enum class eEventType
	{
		Enter = 0
		, Stay = 1
		, Exit = 2
	};

	struct PHYSICS_DLL_STRUCT CollisionEvent
	{
		UserData* _userData1;
		UserData* _userData2;

		Math::Vector3 _impulse;

		uint32 _contactCount;

		std::vector<ContactPoint> _contactPoints;

		eEventType _eventType;
	};

	struct PHYSICS_DLL_STRUCT TriggerEvent
	{
		UserData* _userData1;
		UserData* _userData2;


		eEventType _eventType;
	};

	struct PHYSICS_DLL_STRUCT PhysicsLayer
	{
		uint32 _word0;
		uint32 _word1;
		uint32 _word2;
		uint32 _word3;
	};

	struct PHYSICS_DLL_STRUCT DebugPoint
	{
		DebugPoint() 
		: pos(Math::Vector3::Zero)
		, color(0)
		{}
		DebugPoint(const Math::Vector3& p, const uint32& c)
			: pos(p), color(c) {}

		Math::Vector3 pos;
		uint32 color;
	};

	struct PHYSICS_DLL_STRUCT DebugLine
	{
		DebugLine()
		: pos0(Math::Vector3::Zero)
		, pos1(Math::Vector3::Zero)
		, color0(0) 
		, color1(0) 
		{}
		DebugLine(const Math::Vector3& p0, const Math::Vector3& p1, const uint32& c)
			: pos0(p0), color0(c), pos1(p1), color1(c) {}

		Math::Vector3 pos0;
		uint32 color0;
		Math::Vector3 pos1;
		uint32 color1;
	};

	struct PHYSICS_DLL_STRUCT DebugTriangle
	{
		DebugTriangle()
		: pos0(Math::Vector3::Zero)
		, pos1(Math::Vector3::Zero)
		, pos2(Math::Vector3::Zero)
		, color0(0)
		, color1(0) 
		, color2(0) 
		{}
		DebugTriangle(const Math::Vector3& p0, const Math::Vector3& p1, const Math::Vector3& p2, const uint32& c)
			: pos0(p0), color0(c), pos1(p1), color1(c), pos2(p2), color2(c) {}

		Math::Vector3 pos0;
		uint32 color0;
		Math::Vector3 pos1;
		uint32 color1;
		Math::Vector3 pos2;
		uint32 color2;
	};
}