#pragma once

#include "Common.h"
#include "Struct/VertexAttribute.h"

namespace Utility
{
	struct KeyFrame
	{
		float _time;

		Math::Vector3 _pos;
		Math::Quaternion _rot;
		Math::Vector3 _scale;
	};

	struct AnimationClipData
	{
		tstring _clipName;
		std::vector<KeyFrame> _keyFrames;


		float _frameRate; // 엔진에서 60프레임으로 돌린다고 했을때, 한 프레임을 얼만큼 보여줄건지에 대한 비율
		uint32 _tickPerFrame;
		uint32 _totalKeyFrame;
		uint32 _startKeyFrame;
		uint32 _endKeyFrame;
	};

	struct MeshData
	{
		tstring _meshName;

		std::vector<Common::VertexAttribute> _vertexAttribute;

		std::vector<std::vector<uint32>> _indexAttributes;

		std::vector<tstring> _materials;

		bool _isSkin = false;
	};

	struct MaterialData
	{
		tstring _matarialName;

		Math::Vector4 _ambient;
		Math::Vector4 _diffuse;
		Math::Vector4 _specular;
		Math::Vector4 _emissive;	// 발광색

		float _emissionFactor;
		float _roughness;
		float _metallic;
		float _shininess;	// 빛남의 광도
		float _transparency; // 투명도 
		float _reflectivity; // 반사율

		tstring _albedoMapTexture; // baseColor + opacity
		tstring _normalMapTexture;
		tstring _metalicRoughnessMapTexture; // roughness + metallic + AO 
		tstring _emissiveMapTexture;
		tstring _ambientMapTexture;
		tstring _specularMapTexture;
	};

	struct GameObjectData
	{
		tstring _gameObjectName;

		Math::Matrix _localTM =  Math::Matrix::Identity;
		Math::Matrix _worldTM =  Math::Matrix::Identity;


		bool _hasParent = false;

		tstring _parent;

		bool _hasMesh = false;

		tstring _mesh;

		bool _hasAnimation = false;
	};

	struct PrefabData
	{
		tstring _name;

		std::vector<GameObjectData> _gameObjectDatas;

		std::map<tstring, MeshData> _meshDataMap;

		std::map<tstring, MaterialData> _materialDataMap;

		std::vector<AnimationClipData> _animationClipDatas;
	};
}