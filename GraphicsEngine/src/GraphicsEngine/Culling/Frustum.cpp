#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Culling/Frustum.h"

namespace Graphics
{

	Frustum::Frustum()
	{

	}

	Frustum::~Frustum()
	{

	}

	void Frustum::FrustumUpdate(float posx, float posy, float posz, float forwardx, float forwardy, float forwardz, float upx, float upy, float upz, float fovY, float aspectRatio, float nearZ, float farZ)
	{

	}

	bool Frustum::IsCulling(Math::Vector3* points, Math::Vector3& pos)
	{



		return false;
	}

}