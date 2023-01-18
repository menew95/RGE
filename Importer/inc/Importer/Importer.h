#pragma once

#include "Common.h"

#include "Importer/Export.h"

namespace Utility
{
	struct PrefabData;

	class IMPORTER_DLL_DECLSPEC Importer
	{
	public:
		Importer() = default;
		~Importer() = default;

		void LoadPrefabDataFormFile(const tstring& filePath, PrefabData& prefabData);

	private:
		enum class FileFormat
		{
			FBX,
			UNKNOWN
		};

		FileFormat CheckFileFormat(const tstring& path);

		void LoadPrefabDataFormFBXFile(tstring filePath, PrefabData& prefabData);
	};
}