#pragma once

namespace GameEngine
{
	namespace Core
	{
		enum class MetaData
		{
			Serializable,
			ObjectType,
		};

		enum class MetaDataType
		{
			WSTRING,
			VECTOR2,
			VECTOR3,
			VECTOR4,
			MATRIX,
			UUID,
			BOOL,
			UINT32,
			INT32,
			FLOAT,
		};

		enum class Util
		{
			Check_Vaild
		};

		enum class ObjectType
		{
			GameObject,
			Component,
			Resource,
		};

		enum class ComponentType
		{
			Transform,
		};

		enum class ResourceType
		{
			Mesh,
			Bone,
			Material,
			AnimationClip,
			AudioClip
		};
	}
}