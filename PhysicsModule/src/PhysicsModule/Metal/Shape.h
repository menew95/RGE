#pragma once

#include "PhysicsModule\PhysicsInfo.h"

namespace PhysicsModule
{
	class Shape
	{
	public:
		Shape();
		~Shape();

		bool CreateBoxShape(BoxColliderInfo& boxColliderInfo, physx::PxMaterial* pxMaterial = nullptr);
		bool CreateSphereShape(SphereColliderInfo& sphereColliderInfo, physx::PxMaterial* pxMaterial = nullptr);
		bool CreateCapsuleShape(CapsuleColliderInfo& capsuleColliderInfo, physx::PxMaterial* pxMaterial = nullptr);
		bool CreateMeshShape(MeshColliderInfo& meshColliderInfo, physx::PxMaterial* pxMaterial = nullptr);

		bool CreateHeightFieldShape(HeightFieldColliderInfo& heightFieldInfo, physx::PxMaterial* pxMaterial = nullptr);

		physx::PxShape* GetShape() { return m_pShape; }

		bool SetLayer(physx::PxFilterData& filterData);
		bool SetIsTrigger(bool flag);

		void SetCenter(Math::Vector3 center);
		void SetSize(Math::Vector3 size);
		void SetRaduis(float raduis);
		void SetHeight(float height);
		void SetAxisDirection(eAxisDirection dir);


	private:
		inline bool CheckType(eShapeType shapeType);

		physx::PxMaterial* m_pMaterial;
		physx::PxShape* m_pShape;

		// Todo : ∞ÌπŒ¡ﬂ¿Ã¥Ÿ.
		ColliderInfo* m_ColliderInfo;
	};
}

