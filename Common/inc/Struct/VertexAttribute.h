#pragma once

#include "Math\Math.h"

#include "Common.h"

namespace Common
{
	struct VertexAttribute
	{
		Math::Vector3 _position = Math::Vector3::Zero;
		Math::Vector4 _color = Math::Vector4::One;
		Math::Vector2 _uv = Math::Vector2::Zero;
		Math::Vector3 _normal = Math::Vector3::Zero;
		Math::Vector3 _tangent = Math::Vector3::Zero;
		uint32 _bone[4] = { UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX };
		float _weight[4] = { 0.f, 0.f, 0.f, 0.f };
	};
}

using namespace Common;