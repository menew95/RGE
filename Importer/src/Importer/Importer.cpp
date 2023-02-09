#include "Importer_pch.h"
#include "Importer/Importer.h"
#include "Importer/FBX/FBXImporter.h"

#include "BinarySerializer/BinarySerializer.h"

#include "Deserializer/Deserializer.h"

namespace Utility
{
	void ReconstructionFileName(std::string& name)
	{
		if (name.find("/") != std::string::npos)
		{
			int idx = name.find("/") + 1;

			name.erase(0, idx);

			ReconstructionFileName(name);
		}
	}

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


		std::shared_ptr<BinarySerializer> binarySerializer = std::make_shared<BinarySerializer>();

		std::string saveBinaryPath = "Asset/BinaryFile/";
		std::string _fileName = StringHelper::WStringToString(filePath);
		int idx = filePath.find(L".") + 1;

		size_t dot_pos = filePath.find_last_of(L".");
		if (dot_pos != std::string::npos)
		{
			_fileName = _fileName.substr(0, dot_pos);
		}

		ReconstructionFileName(_fileName);

		binarySerializer->SaveBinaryFile(&prefabData, _fileName, saveBinaryPath);
	}

	bool Importer::Deserialize(MaterialData& materialData, uuid uuid)
	{
		Deserializer _deserializer;
		BinaryData::MaterialData _materialData;

		if (!_deserializer.Deserialize(_materialData, uuid))
			return false;

		materialData._matarialName = StringHelper::StringToWString(_materialData._materialName);

		materialData._ambient = _materialData._ambientColor;
		materialData._diffuse = _materialData._diffuseColor;
		materialData._specular = _materialData._specularColor;
		materialData._emissive = _materialData._emissiveColor;
		
		materialData._albedoMapTexture = StringHelper::StringToWString(_materialData._albedoMapTexture);
		materialData._normalMapTexture = StringHelper::StringToWString(_materialData._normalMapTexture);
		materialData._metalicRoughnessMapTexture = StringHelper::StringToWString(_materialData._metalicRoughnessMapTexture);
		materialData._emissiveMapTexture = StringHelper::StringToWString(_materialData._emissiveMapTexture);
		materialData._ambientMapTexture = StringHelper::StringToWString(_materialData._ambientMapTexture);
		materialData._specularMapTexture = StringHelper::StringToWString(_materialData._specularMapTexture);
		
		materialData._emissionFactor = _materialData._emissionFactor;
		materialData._transparency = _materialData._transparency;
		materialData._reflectivity = _materialData._reflectivity;
		materialData._metallic = _materialData._metallic;
		materialData._roughness = _materialData._roughness;

		return true;
	}

	bool Importer::Deserialize(MeshData& meshData, uuid uuid)
	{
		Deserializer _deserializer;
		BinaryData::MeshData _meshData;

		if (!_deserializer.Deserialize(_meshData, uuid))
			return false;

		meshData._meshName = StringHelper::StringToWString(_meshData.meshName);
		meshData._vertexAttributes = _meshData.meshVertexList;
		meshData._indexAttributes = _meshData.indices;
		meshData._isSkin = _meshData._isSkinned;
		meshData._skinName = StringHelper::StringToWString(_meshData._boneName);

		return true;
	}

	bool Importer::Deserialize(AnimationClipData& animationClip, uuid uuid)
	{
		Deserializer _deserializer;
		BinaryData::AnimationClipData _animationData;

		if (!_deserializer.Deserialize(_animationData, uuid))
			return false;

		animationClip._clipName = StringHelper::StringToWString(_animationData._clipName);

		animationClip._frameRate = _animationData._frameRate;
		animationClip._tickPerFrame = _animationData._tickPerFrame;
		animationClip._totalKeyFrame = _animationData._totalKeyFrame;
		animationClip._startKeyFrame = _animationData._startKeyFrame;
		animationClip._endKeyFrame = _animationData._endKeyFrame;

		animationClip._snapList.reserve(_animationData._snapList.size());

		for (auto& _snapData : _animationData._snapList)
		{
			Utility::AnimationSnapData _snap;

			_snap._nodeName = StringHelper::StringToWString(_snapData._nodeName);
			_snap._parentNodeName = StringHelper::StringToWString(_snapData._parentNodeName);

			_snap._keyFrameList = _snapData._keyFrameList;

			animationClip._snapList.emplace_back(_snap);
		}

		return true;
	}

	bool Importer::Deserialize(std::vector<BoneData>& boneDataList, uuid uuid)
	{
		Deserializer _deserializer;
		BinaryData::SkinnedData _skinnedData;

		if (!_deserializer.Deserialize(_skinnedData, uuid))
			return false;
		
		boneDataList.reserve(_skinnedData._boneDatas.size());

		for (auto& _bone : _skinnedData._boneDatas)
		{
			Utility::BoneData _boneData;

			_boneData._boneName = StringHelper::StringToWString(_bone._boneName);
			_boneData._parentBoneName = StringHelper::StringToWString(_bone._parentBoneName);
			_boneData._parentBoneIndex = _bone._parentIndex;

			_boneData._offsetTM = _bone._offsetMatrix;
			_boneData._localTM = _bone._localMatrix;
			_boneData._worldTM = _bone._worldMatrix;

			boneDataList.push_back(_boneData);
		}

		return true;
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