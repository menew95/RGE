#pragma once
#include <iostream>
#include <fstream>
#include <boost/serialization/access.hpp>

#include "Math/Math.h"

#include "Importer/ImportInfo.h"

namespace fs = std::filesystem;

namespace BinaryData
{
	class MaterialData
	{
	public:
		friend boost::serialization::access;

		MaterialData() = default;

		MaterialData(std::string materialName
			, std::string albedoMap, std::string normalMap, std::string metalicRoughnessMap
			, std::string emissiveMap, std::string ambientMap, std::string specularMap
			, Math::Vector4 ambientColor, Math::Vector4 diffuseColor, Math::Vector4 specularColor, Math::Vector4 emissiveColor
			//, Float4 ambientColor, Float4 diffuseColor, Float4 specularColor, Float4 emissiveColor
			, float emissionFactor, float transparency, float reflectivity, float metallic, float roughness)
			: _materialName(std::move(materialName))
			
			, _albedoMapTexture(std::move(albedoMap))
			, _normalMapTexture(std::move(normalMap))
			, _metalicRoughnessMapTexture(std::move(metalicRoughnessMap))
			, _emissiveMapTexture(std::move(emissiveMap))
			, _ambientMapTexture(std::move(ambientMap))
			, _specularMapTexture(std::move(specularMap))

			, _ambientColor(std::move(ambientColor))
			, _diffuseColor(std::move(diffuseColor))
			, _specularColor(std::move(specularColor))
			, _emissiveColor(std::move(emissiveColor))

			, _emissionFactor(std::move(emissionFactor))
			, _transparency(std::move(transparency))
			, _reflectivity(std::move(reflectivity))
			, _metallic(std::move(metallic))
			, _roughness(std::move(roughness))
		{}

	public:
		std::string _materialName = "";

		std::string _albedoMapTexture = "";
		std::string _normalMapTexture = "";
		std::string _metalicRoughnessMapTexture = "";
		std::string _emissiveMapTexture = "";
		std::string _ambientMapTexture = "";
		std::string _specularMapTexture = "";

		Math::Vector4 _ambientColor = { 0.f, 0.f, 0.f, 0.f };
		Math::Vector4 _diffuseColor = { 0.f, 0.f, 0.f, 0.f };
		Math::Vector4 _specularColor = { 0.f, 0.f, 0.f, 0.f };
		Math::Vector4 _emissiveColor = { 0.f, 0.f, 0.f, 0.f };

		float _emissionFactor = 0.f;
		float _transparency = 0.f;
		float _reflectivity = 0.f;

		float _metallic = 0.f;
		float _roughness = 0.f;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _materialName;

			ar& _albedoMapTexture;
			ar& _normalMapTexture;
			ar& _metalicRoughnessMapTexture;
			ar& _ambientMapTexture;
			ar& _emissiveMapTexture;
			ar& _specularMapTexture;

			ar& _ambientColor;
			ar& _diffuseColor;
			ar& _specularColor;
			ar& _emissiveColor;

			ar& _transparency;
			ar& _reflectivity;

			ar& _metallic;
			ar& _roughness;
		}
	};

	class MeshData
	{
	public:
		friend boost::serialization::access;

		MeshData() = default;

		MeshData(std::string _meshName, std::vector<Common::VertexAttribute> _meshVertexList
			, std::vector<std::vector<uint32>> _indices)
			: meshName(std::move(_meshName))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, _isSkinned(false)
		{}

		MeshData(std::string _meshName, std::vector<Common::VertexAttribute> _meshVertexList
			, std::vector<std::vector<uint32>> _indices, std::string bone)
			: meshName(std::move(_meshName))
			, meshVertexList(std::move(_meshVertexList))
			, indices(std::move(_indices))
			, _isSkinned(true)
			, _boneName(std::move(bone))
		{}

	public:
		std::string								meshName = "";
		std::vector<Common::VertexAttribute>	meshVertexList;
		std::vector<std::vector<uint32>>		indices;

		bool _isSkinned;
		std::string _boneName;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& meshName;
			ar& meshVertexList;
			ar& indices;
			ar& _isSkinned;
			ar& _boneName;
		}
	};

	class BoneData
	{
	public:
		friend boost::serialization::access;

		BoneData() = default;

		BoneData(std::string boneName, std::string parentBoneName, int parentIndex
			, Math::Matrix offsetMatrix, Math::Matrix localMatrix, Math::Matrix worldMatrix)
			: _boneName(std::move(boneName))
			, _parentBoneName(std::move(boneName))
			, _parentIndex(std::move(parentIndex))
			, _offsetMatrix(std::move(offsetMatrix))
			, _localMatrix(std::move(localMatrix))
			, _worldMatrix(std::move(worldMatrix))
		{}

	public:
		std::string	_boneName = "";
		std::string	_parentBoneName = "";

		short 		_parentIndex = 0;

		Math::Matrix	_offsetMatrix;
		Math::Matrix	_localMatrix;
		Math::Matrix	_worldMatrix;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _boneName;
			ar& _parentBoneName;
			ar& _parentIndex;
			ar& _offsetMatrix;
			ar& _localMatrix;
			ar& _worldMatrix;
		}
	};

	struct SkinnedData
	{
		std::vector<BoneData> _boneDatas;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _boneDatas;
		}
	};

	struct AnimationSnapData
	{
		AnimationSnapData() = default;

		AnimationSnapData(std::string name, std::string parent, std::vector<Utility::KeyFrame> keyFrameList)
			: _nodeName(std::move(name))
			, _parentNodeName(std::move(parent))
			, _keyFrameList(std::move(keyFrameList))
		{}

		std::string _nodeName;
		std::string _parentNodeName;

		std::vector<Utility::KeyFrame> _keyFrameList;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _nodeName;
			ar& _parentNodeName;
			ar& _keyFrameList;
		}
	};

	struct AnimationClipData
	{
		AnimationClipData() = default;

		AnimationClipData(std::string clipName, std::vector<AnimationSnapData> snapList
			, float frameRate, uint32 tickPerTime, uint32 totalKeyFrame, uint32 startKeyFrame, uint32 endKeyFrame)
			: _clipName(clipName)
			, _snapList(snapList)
			, _frameRate(frameRate)
			, _tickPerFrame(tickPerTime)
			, _totalKeyFrame(totalKeyFrame)
			, _startKeyFrame(startKeyFrame)
			, _endKeyFrame(endKeyFrame)
		{}

		std::string _clipName;
		std::vector<AnimationSnapData> _snapList;


		float _frameRate; // 엔진에서 60프레임으로 돌린다고 했을때, 한 프레임을 얼만큼 보여줄건지에 대한 비율
		uint32 _tickPerFrame;
		uint32 _totalKeyFrame;
		uint32 _startKeyFrame;
		uint32 _endKeyFrame;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _clipName;
			ar& _snapList;

			ar& _frameRate;
			ar& _tickPerFrame;
			ar& _totalKeyFrame;
			ar& _startKeyFrame;
			ar& _endKeyFrame;
		}
	};
	
	class ModelData
	{
	public:
		friend boost::serialization::access;

		ModelData() = default;

		ModelData(std::vector<std::string> _meshInfoList, std::vector<std::string> _materialList,
			std::vector<std::string> _animationList, std::vector<BoneData> _boneInfoList, bool _isSkinnedAnimation)
			: meshInfoList(std::move(_meshInfoList))
			, materialList(std::move(_materialList))
			, animationClipList(std::move(_animationList))
			, boneInfoList(std::move(_boneInfoList))
			, isSkinnedAnimation(_isSkinnedAnimation)
		{}

	public:
		std::string name;

		std::vector<std::string> meshInfoList;

		std::vector<std::string> materialList;

		std::vector<std::string> animationClipList;

		std::vector<BoneData> boneInfoList;

		bool isSkinnedAnimation = false;

	private:
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;

			ar& meshInfoList;
			ar& materialList;
			ar& animationClipList;
			ar& boneInfoList;

			ar& isSkinnedAnimation;
		}
	};
}