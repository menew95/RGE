#include "Importer_pch.h"
#include "Importer/Importer.h"
#include "Importer/FBX/FBXImporter.h"

namespace Utility
{
	void Importer::LoadPrefabDataFormFile(const tstring& filePath, PrefabData& prefabData)
	{
		auto _format = CheckFileFormat(filePath);

		switch (_format)
		{
			case Utility::Importer::FileFormat::FBX:
				LoadPrefabDataFormFBXFile(filePath, prefabData);
				break;
			case Utility::Importer::FileFormat::UNKNOWN:
			default:
				assert(false);
				break;
		}

	}

	Importer::FileFormat Importer::CheckFileFormat(const tstring& path)
	{
		if (static_cast<uint32>(path.length()) > 1)
		{
			size_t extensionStartPoint = path.find_last_of('.') + (size_t)1;

			auto fileFormat = path.substr(extensionStartPoint);

			if (fileFormat == TEXT("fbx") || fileFormat == TEXT("FBX"))
			{
				return FileFormat::FBX;
			}
		}

		return FileFormat::UNKNOWN;
	}

	void Importer::LoadPrefabDataFormFBXFile(tstring filePath, PrefabData& prefabData)
	{
		FBXImporter _fbxImporter;

		_fbxImporter.LoadFile(filePath, prefabData);
	}
}