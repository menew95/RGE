#pragma once

namespace Graphics
{
	struct StageFlags
	{
		enum eStageFlags
		{
			VS = (1 << 0),
			HS = (1 << 1),
			DS = (1 << 2),
			GS = (1 << 3),
			PS = (1 << 4),
			CS = (1 << 5),

			AllTessStages = (HS | DS),

			AllGraphicsStages = (VS | AllTessStages | GS | PS),

			AllStages = (AllGraphicsStages | CS),
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
		Reserved0	= 0xFFF0,
	};

	enum class ShaderSourceType
	{
		HLSL	= 0,
		CSO		= 1,
	};

	struct ShaderMacro
	{
		ShaderMacro() = default;
		ShaderMacro(const ShaderMacro&) = default;
		ShaderMacro& operator = (const ShaderMacro&) = default;

		inline ShaderMacro(const char* name, const char* definition = nullptr) :
			name{ name },
			definition{ definition }
		{
		}

		const char* name = nullptr;

		const char* definition = nullptr;
	};

	struct ShaderDesc
	{
		ShaderType _shaderType;
		ShaderSourceType _sourceType;

		tstring _filePath;
		uint32 _sourceSize = 0;

		const char* _entryPoint = nullptr;
		const char* _profile = nullptr;

		const ShaderMacro* defines = nullptr;

		uint32 _flags = 0;
	};


	struct ShaderProgramDesc
	{
		StageFlags _flags;
	};
}