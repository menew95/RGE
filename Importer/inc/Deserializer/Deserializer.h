#pragma once

#include "Common.h"

#include "BinarySerializer/FBXBinaryLayout.h"

#include "Common/StringHelper.h"


namespace Utility
{
	class Deserializer
	{
	public:
		template<typename T>
		bool Deserialize(T& data, uuid uuid)
		{
			static_assert(false, "need template specialization for T");
			return false;
		};

		template<>
		bool Deserialize(BinaryData::MaterialData& meterialData, uuid uuid);

		template<>
		bool Deserialize(BinaryData::MeshData& meshData, uuid uuid);

		template<>
		bool Deserialize(BinaryData::SkinnedData& skinnedData, uuid uuid);

		template<>
		bool Deserialize(BinaryData::AnimationClipData& animationClipData, uuid uuid);
	};
}