#pragma once

#include "Math\Math.h"

#include "Common.h"



namespace boost
{
	namespace serialization
	{
		class access;
	}

}

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

		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _position;
			ar& _color;
			ar& _uv;
			ar& _normal;
			ar& _tangent;
			ar& _bone;
			ar& _weight;
		}
	};
}

using namespace Common;