#pragma once

#include "Common.h"

namespace Graphics
{
	class Frustum
	{
	public:
		Frustum(Math::Matrix proj, Math::Matrix camWorld);
		~Frustum();

		bool IsIntersects(Math::Matrix world, Math::Vector3 boundMin, Math::Vector3 boundMax);

	private:
		BoundingFrustum m_Frustum;
	};
}