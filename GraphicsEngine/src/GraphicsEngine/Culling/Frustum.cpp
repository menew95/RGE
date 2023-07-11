#include "GraphicsEnginePCH.h"

#include "GraphicsEngine/Culling/Frustum.h"

namespace Graphics
{

	Frustum::Frustum(Math::Matrix proj, Math::Matrix camWorld)
		: m_Frustum(proj)
	{
		m_Frustum.Transform(m_Frustum, camWorld);
	}

	Frustum::~Frustum()
	{

	}

	bool Frustum::IsIntersects(Math::Matrix world, Math::Vector3 boundMin, Math::Vector3 boundMax)
	{
		auto _center = (boundMin + boundMax) * 0.5f;
		auto _extents = boundMax - _center;

		BoundingOrientedBox _boundingOrientedBox;
		_boundingOrientedBox.Center = _center;
		_boundingOrientedBox.Extents = _extents;

		_boundingOrientedBox.Transform(_boundingOrientedBox, world);

		return m_Frustum.Intersects(_boundingOrientedBox);
	}
}