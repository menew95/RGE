#pragma once

#include "Common.h"
#include "Struct/VertexAttribute.h"

namespace boost
{
	namespace serialization
	{
		class access;
	}
}

namespace Utility
{
	struct KeyFrame
	{
		float _time;

		Math::Vector3 _pos;
		Math::Quaternion _rot;
		Math::Vector3 _scale;

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& _time;
			ar& _pos;
			ar& _rot;
			ar& _scale;
		}
	};

	struct AnimationSnapData
	{
		tstring _nodeName;
		tstring _parentNodeName;

		std::vector<KeyFrame> _keyFrameList;
	};

	struct AnimationClipData
	{
		tstring _clipName;
		std::vector<AnimationSnapData> _snapList;

		float _frameRate; // �������� 60���������� �����ٰ� ������, �� �������� ��ŭ �����ٰ����� ���� ����
		uint32 _tickPerFrame;
		uint32 _totalKeyFrame;
		uint32 _startKeyFrame;
		uint32 _endKeyFrame;
	};

	struct MeshData
	{
		tstring _meshName;

		std::vector<Common::VertexAttribute> _vertexAttributes;

		std::vector<std::vector<uint32>> _indexAttributes;

		std::vector<tstring> _materials;

		bool _isSkin = false;

		tstring _skinName;
		std::vector<tstring> _boneList;
	};

	struct MaterialData
	{
		tstring _matarialName;

		Math::Vector4 _ambient;
		Math::Vector4 _diffuse;
		Math::Vector4 _specular;
		Math::Vector4 _emissive;	// �߱���

		float _emissionFactor;
		float _shininess;	// ������ ����
		float _transparency; // ���� 
		float _reflectivity; // �ݻ���
		float _roughness;
		float _metallic;

		tstring _albedoMapTexture; // baseColor + opacity
		tstring _normalMapTexture;
		tstring _metalicRoughnessMapTexture; // roughness + metallic + AO 
		tstring _emissiveMapTexture;
		tstring _ambientMapTexture;
		tstring _specularMapTexture;
	};

	struct BoneData
	{
		tstring _boneName;
		tstring _parentBoneName;
		int _parentBoneIndex;

		Math::Matrix _localTM = Math::Matrix::Identity;
		Math::Matrix _worldTM = Math::Matrix::Identity;

		Math::Matrix _offsetTM;
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

		bool _isBone = false;
	};

	struct PrefabData
	{
		tstring _filePath;
		tstring _name;

		GameObjectData _rootObject;
		GameObjectData _amatureObject;

		std::vector<GameObjectData> _gameObjectDatas;

		std::vector<BoneData> _boneDatas;

		std::map<tstring, MeshData> _meshDataMap;

		std::map<tstring, MaterialData> _materialDataMap;

		std::vector<AnimationClipData> _animationClipDatas;
	};
}