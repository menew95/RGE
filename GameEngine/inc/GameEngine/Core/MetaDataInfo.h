#pragma once

namespace GameEngine
{
	namespace Core
	{
		enum class MetaData
		{
			Serializable,
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
	}
}