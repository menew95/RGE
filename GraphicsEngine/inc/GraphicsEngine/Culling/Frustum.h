#pragma once

#include "Common.h"

#include <DirectXCollision.h>

namespace Graphics
{
	class Frustum
	{
	public:
		Frustum();
		~Frustum();

		// ����ü�� ����� ������Ʈ
		void FrustumUpdate(float posx, float posy, float posz, float forwardx,
			float forwardy, float forwardz, float upx, float upy, float upz,
			float fovY, float aspectRatio, float nearZ, float farZ);

		bool IsCulling(Math::Vector3* points, Math::Vector3& pos);
	private:
		
	};
}