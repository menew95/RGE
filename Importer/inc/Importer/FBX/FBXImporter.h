#pragma once

#include "Common.h"

#include "Importer/ImportInfo.h"

namespace fbxsdk
{
	class FbxManager;
	class FbxImporter;
	class FbxScene;
	class FbxNode;
	class FbxMesh;
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

		void LoadFile(const tstring& filePath, PrefabData& prefabData);

	private:
		void FBXInitialize();

		void FindAnimationData(PrefabData& prefabData);
		void LoadAnimationData(fbxsdk::FbxNode* node, PrefabData& prefabData);

		void LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, PrefabData& prefabData, MeshData& meshData);

		void LoadMesh(PrefabData& prefabData, fbxsdk::FbxMesh* meshNode, MeshData& meshData, int meshCnt);

		Math::Vector3 GetNormal(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter);

		void GetTangent(MeshData& meshInfo, int meshCnt);

		Math::Vector2 GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter);

		void FindBoneNode(fbxsdk::FbxNode* node, PrefabData& prefabData, int idx, int parentIdx);

		void TraversalNode(fbxsdk::FbxNode* node, PrefabData& prefabData);

		tstring GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty);

		int32 FindBoneIndex(std::string boneName, PrefabData& prefabData);

		Math::Matrix GetLocalMatrix(fbxsdk::FbxNode* node, bool lh = true);
		Math::Matrix GetWorldMatrix(fbxsdk::FbxNode* node, bool lh = true);

		bool m_IsNegativeScaleFlag = false;
	};
}