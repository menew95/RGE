#pragma once

#include "Common.h"

#include "Importer/Export.h"

namespace Utility
{
	struct PrefabData;
	struct MeshData;
	struct MaterialData;
	struct AnimationClipData;
	struct BoneData;

	class IMPORTER_DLL_DECLSPEC Importer
	{
	public:
		Importer() = default;
		~Importer() = default;

		void LoadPrefabDataFormFile(const tstring& filePath, PrefabData& prefabData);

		bool Deserialize(MaterialData& materialData, uuid uuid);
		bool Deserialize(MeshData& meshData, uuid uuid);
		bool Deserialize(AnimationClipData& animationClip, uuid uuid);
		bool Deserialize(std::vector<Utility::BoneData>& boneDataList, uuid uuid);

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