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

	enum class ShaderType
	{
		Pixel		= 0,
		Vertex		= 1,
		Geometry	= 2,
		Hull		= 3,
		Domain		= 4,
		Compute		= 5,
	};

	enum class ShaderSourceType
	{
		HLSL	= 0,
		CSO		= 1,
	};

	struct ShaderDesc
	{
		ShaderType _shaderType;
	};


	struct ShaderProgramDesc
	{
		StageFlags _flags;
	};
}