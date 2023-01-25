#pragma once

#include "Math\Math.h"

#include "Common.h"

namespace Common
{
	struct VertexAttribute
	{
		Math::Vector3 _position;
		Math::Vector4 _color;
		Math::Vector2 _uv;
		Math::Vector3 _normal;
		Math::Vector3 _tangent;
		uint32 _bone[4];
		float _weight[4];
	};
}

using namespace Common;