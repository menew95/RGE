#include "PhysicsModule\PhysicsPCH.h"
#include "PhysicsModule\PhysicsImpl.h"
#include "PhysicsModule\Metal\Shape.h"


PhysicsModule::Shape::Shape()
{

}

PhysicsModule::Shape::~Shape()
{
	SAFE_RELEASE(m_pShape)
}

bool PhysicsModule::Shape::CreateBoxShape(BoxColliderInfo& boxColliderInfo, physx::PxMaterial* pxMaterial)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (boxColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxBoxGeometry(boxColliderInfo._size.x * 0.5f, boxColliderInfo._size.y * 0.5f, boxColliderInfo._size.z * 0.5f),
		*pxMaterial, true, flags);

	physx::PxFilterData filterData;
	filterData.word0 = 0x00000001;//자기자신
	filterData.word1 = 0xFFFFFFFF;//다른 상대 오브젝트들
	m_pShape->setSimulationFilterData(filterData);

	m_ColliderInfo = &boxColliderInfo;
	return true;
}

bool PhysicsModule::Shape::CreateSphereShape(SphereColliderInfo& sphereColliderInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (sphereColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxSphereGeometry(sphereColliderInfo._radius),
		*pxMaterial, true, flags);

	m_ColliderInfo = &sphereColliderInfo;

	return true;
}

bool PhysicsModule::Shape::CreateCapsuleShape(CapsuleColliderInfo& capsuleColliderInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

	if (capsuleColliderInfo._isTrigger)
	{
		flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
	}

	m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxCapsuleGeometry(capsuleColliderInfo._radius, capsuleColliderInfo._height),
		*pxMaterial, true, flags);

	m_ColliderInfo = &capsuleColliderInfo;

	return true;
}

bool PhysicsModule::Shape::CreateMeshShape(MeshColliderInfo& meshColliderInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	physx::PxShapeFlags flags = physx::PxShapeFlag::eVISUALIZATION | physx::PxShapeFlag::eSCENE_QUERY_SHAPE;


	if (meshColliderInfo._convex)
	{
		if (meshColliderInfo._isTrigger)
		{
			flags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
		}

		physx::PxConvexMeshDesc _convexDesc;

		if (meshColliderInfo._triCount == 0 ||
			meshColliderInfo._triStride == 0 ||
			meshColliderInfo._triData == nullptr)
		{
			_convexDesc.points.count = meshColliderInfo._count;
			_convexDesc.points.stride = meshColliderInfo._stride;
			_convexDesc.points.data = meshColliderInfo._meshData;

			_convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;// | physx::PxConvexFlag::eDISABLE_MESH_VALIDATION;
			_convexDesc.vertexLimit = 255;
		}
		else
		{
			_convexDesc.polygons.count = meshColliderInfo._count;
			_convexDesc.polygons.stride = meshColliderInfo._stride;
			_convexDesc.polygons.data = meshColliderInfo._meshData;

			_convexDesc.indices.count = meshColliderInfo._triCount;
			_convexDesc.indices.stride = meshColliderInfo._triStride;
			_convexDesc.indices.data = meshColliderInfo._triData;
		}

		physx::PxDefaultMemoryOutputStream _buf;

		physx::PxConvexMeshCookingResult::Enum _result;

		if (!PhysicsSDK->GetCooking()->cookConvexMesh(_convexDesc, _buf, &_result))
		{
			AssertMessageBox(false, "Convex mesh cooking faild");
			return false;
		}

		if (_result == physx::PxConvexMeshCookingResult::eFAILURE
			|| _result == physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED)
		{
			AssertMessageBox(false, "Convex mesh cooking faild");
			return false;
		}


		physx::PxDefaultMemoryInputData _input(_buf.getData(), _buf.getSize());
		physx::PxConvexMesh* _convexMesh = PhysicsSDK->GetPhysics()->createConvexMesh(_input);

		m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxConvexMeshGeometry(_convexMesh),
			*pxMaterial, true, flags);
	}
	else
	{
		flags |= physx::PxShapeFlag::eSIMULATION_SHAPE;

		physx::PxTriangleMeshDesc _triangleDesc;

		_triangleDesc.points.count = meshColliderInfo._count;
		_triangleDesc.points.stride = meshColliderInfo._stride;
		_triangleDesc.points.data = meshColliderInfo._meshData;

		_triangleDesc.triangles.count = meshColliderInfo._triCount;
		_triangleDesc.triangles.stride = meshColliderInfo._triStride;
		_triangleDesc.triangles.data = meshColliderInfo._triData;
		
		physx::PxDefaultMemoryOutputStream _buf;
		physx::PxTriangleMeshCookingResult::Enum _result;

		if (!PhysicsSDK->GetCooking()->cookTriangleMesh(_triangleDesc, _buf, &_result))
		{
			AssertMessageBox(false, "Triangle mesh cooking faild");
			return false;
		}

		physx::PxDefaultMemoryInputData _readBuffer(_buf.getData(), _buf.getSize());
		
		physx::PxTriangleMesh* _triMesh = PhysicsSDK->GetPhysics()->createTriangleMesh(_readBuffer);

		m_pShape = PhysicsSDK->GetPhysics()->createShape(physx::PxTriangleMeshGeometry(_triMesh),
			*pxMaterial, true, flags);
	}

	m_ColliderInfo = &meshColliderInfo;

	return true;
}

bool PhysicsModule::Shape::CreateHeightFieldShape(HeightFieldColliderInfo& heightFieldInfo, physx::PxMaterial* pxMaterial /*= nullptr*/)
{
	using namespace physx;

	assert(false);

	/*PxHeightFieldDesc _hfDesc;
	_hfDesc.format = PxHeightFieldFormat::eS16_TM;
	_hfDesc.nbColumns = heightFieldInfo._column;
	_hfDesc.nbRows = heightFieldInfo._row;
	_hfDesc.samples.data = samples;
	_hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField* aHeightField = theCooking->createHeightField(_hfDesc,
		thePhysics->getPhysicsInsertionCallback());*/

	return true;
}

bool PhysicsModule::Shape::SetLayer(physx::PxFilterData& filterData)
{
	NullCheck(m_pShape);

	m_pShape->setSimulationFilterData(filterData);

	physx::PxFilterData _queryFilter;
	_queryFilter.word0 = filterData.word0;

	m_pShape->setQueryFilterData(filterData);
	return true;
}

bool PhysicsModule::Shape::SetIsTrigger(bool flag)
{
	NullCheck(m_pShape);

	if (CheckType(eShapeType::MESH) && ! reinterpret_cast<MeshColliderInfo*>(m_ColliderInfo)->_convex)
	{
		return false;
	}

	if(CheckType(eShapeType::HEIGHTFIELD))
	{
		return false;
	}

	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !flag);
	m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, flag);

	return true;
}

void PhysicsModule::Shape::SetCenter(Math::Vector3 center)
{
	// 이후에 비트 연산으로 바꾸고싶다.
	if (CheckType(eShapeType::MESH))
		return;

	NullCheck(m_pShape);

	physx::PxTransform _pose;

	_pose.p = Vector3ToPxVec3(center);
	_pose.q = physx::PxQuat(physx::PxIDENTITY::PxIdentity);
	m_pShape->setLocalPose(_pose);
}

void PhysicsModule::Shape::SetSize(Math::Vector3 size)
{
	// 이후에 비트 연산으로 바꾸고싶다.
	if (!CheckType(eShapeType::BOX))
		return;

	NullCheck(m_pShape)


	auto geo = physx::PxBoxGeometry(physx::PxVec3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));

	m_pShape->setGeometry(geo);
}

void PhysicsModule::Shape::SetRaduis(float raduis)
{

}

void PhysicsModule::Shape::SetHeight(float height)
{

}

void PhysicsModule::Shape::SetAxisDirection(eAxisDirection dir)
{

}

bool PhysicsModule::Shape::CheckType(eShapeType shapeType)
{
	return (shapeType == m_ColliderInfo->_shapeType);
}
