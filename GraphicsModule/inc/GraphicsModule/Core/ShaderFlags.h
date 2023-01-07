#pragma once

namespace Graphics
{
	struct StageFlags
	{
		enum
		{
			VS = (1 << 0),
			HS = (1 << 1),
			DS = (1 << 2),
			GS = (1 << 3),
			PS = (1 << 4),
			CS = (1 << 5),
		};
	};

	struct ShaderDesc
	{

	};
}