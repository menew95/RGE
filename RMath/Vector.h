#pragma once

#include "Math.h"
#include "Types.h"

namespace RMath
{
	struct RVector
	{
		union
		{
			float f32[4];
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};


	};
}