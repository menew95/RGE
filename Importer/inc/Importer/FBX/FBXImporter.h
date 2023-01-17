#pragma once

#include "Common.h"

#include "Importer/ImportInfo.h"

namespace fbxsdk
{
	class FbxManager;
	class FbxImporter;
	class FbxScene;
	class FbxNode;
	class FbxSurfaceMaterial;
}

namespace Utility
{

	class FBXImporter
	{
	public:
		FBXImporter();
		~FBXImporter();

		std::unique_ptr<fbxsdk::FbxManager, void(*)(fbxsdk::FbxManager*)> m_FbxManager;
		std::unique_ptr<fbxsdk::FbxImporter, void(*)(fbxsdk::FbxImporter*)> m_FbxImporter;
		std::unique_ptr<fbxsdk::FbxScene, void(*)(fbxsdk::FbxScene*)> m_FbxScene;

		void LoadFile(tstring filePath, PrefabData& prefabData);

	private:
		void FBXInitialize();

		void LoadAnimation(PrefabData& prefabData);

		void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, PrefabData& prefabData, MeshData& meshData);

		void TraversalNode(fbxsdk::FbxNode* node);

		tstring GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);
	};
}