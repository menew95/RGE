#include "Importer_pch.h"
#include "BinarySerializer/BinarySerializer.h"
#include "Importer/ImportInfo.h"


#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"
//#include "boost/serialization/shared_ptr.hpp"
#include "boost/archive/binary_iarchive.hpp" 
#include "boost/archive/binary_oarchive.hpp" 
//#include "boost/asio/streambuf.hpp"
//#include "boost/iostreams/filtering_stream.hpp"
//#include "boost/iostreams/device/back_inserter.hpp"
//#include "boost/iostreams/copy.hpp"
//#include "boost/iostreams/device/back_inserter.hpp"
//#include "boost/iostreams/stream_buffer.hpp"
#include "boost/iostreams/stream.hpp"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/zlib.hpp"
#include "boost/iostreams/device/file.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "Common/StringHelper.h"

namespace Utility
{
	void BinarySerializer::SaveBinaryFile(PrefabData* prefabData, std::string fileName, std::string path)
	{
 		std::string modelPathStr = "Model/";
 		std::string materialPathStr = "Material/";
 		std::string meshPathStr = "Mesh/";
 		std::string animationPathStr = "Animation/";
 		std::string bonePathStr = "Bone/";

		try {
			// 머터리얼부터 시리얼라이즈

			/*std::vector<tstring> materialList;

			rapidjson::Document::AllocatorType allocator = document.GetAllocator();
			rapidjson::Value matArray(rapidjson::kArrayType);*/

			for (auto& _matPair : prefabData->_materialDataMap)
			{
				bool isMat = false;

				std::string _materialName = StringHelper::WStringToString(_matPair.second._matarialName);

				std::string _albedoMapTexture = StringHelper::WStringToString(_matPair.second._albedoMapTexture);
				std::string _normalMapTexture = StringHelper::WStringToString(_matPair.second._normalMapTexture);
				std::string _metalicRoughnessMapTexture = StringHelper::WStringToString(_matPair.second._metalicRoughnessMapTexture);
				std::string _emissiveMapTexture = StringHelper::WStringToString(_matPair.second._emissiveMapTexture);
				std::string _ambientMapTexture = StringHelper::WStringToString(_matPair.second._ambientMapTexture);
				std::string _specularMapTexture = StringHelper::WStringToString(_matPair.second._specularMapTexture);

				BinaryData::MaterialData binaryMaterial(_materialName
					, _albedoMapTexture, _normalMapTexture, _metalicRoughnessMapTexture
					, _emissiveMapTexture, _ambientMapTexture, _specularMapTexture
					, _matPair.second._ambient, _matPair.second._diffuse, _matPair.second._specular, _matPair.second._emissive
					, _matPair.second._emissionFactor, _matPair.second._transparency, _matPair.second._reflectivity
					, _matPair.second._metallic, _matPair.second._roughness);

				// 모델 데이터에 추가할 머터리얼 이름
				//materialList.emplace_back(binaryMaterial._materialName);

				// .mat BinaryFile 추가
				std::ofstream mat_ofs(path + materialPathStr + binaryMaterial._materialName + ".mat", std::ios_base::binary);
				boost::iostreams::filtering_stream<boost::iostreams::output> matBuffer;
				matBuffer.push(boost::iostreams::zlib_compressor());
				matBuffer.push(mat_ofs);
				boost::archive::binary_oarchive oaMat(matBuffer);
				oaMat << binaryMaterial;
			}

			if (prefabData->_boneDatas.size() > 0)
			{
				BinaryData::SkinnedData _skinnedData;

				for (auto& bone : prefabData->_boneDatas)
				{
					std::string _boneName = StringHelper::WStringToString(bone._boneName);
					std::string _parentBoneName = StringHelper::WStringToString(bone._parentBoneName);

					BinaryData::BoneData boneData(_boneName, _parentBoneName, bone._parentBoneIndex, bone._offsetTM, bone._localTM, bone._worldTM);

					_skinnedData._boneDatas.emplace_back(boneData);
				}

				std::ofstream anim_ofs(path + bonePathStr + fileName + ".bone", std::ios_base::binary);
				boost::iostreams::filtering_stream<boost::iostreams::output> boneBuffer;
				boneBuffer.push(boost::iostreams::zlib_compressor());
				boneBuffer.push(anim_ofs);

				boost::archive::binary_oarchive oaAnim(boneBuffer);
				oaAnim << _skinnedData;
			}

			//// 메시 시리얼라이즈
			std::vector<std::string> meshInfoList;

			for (auto& _meshPair : prefabData->_meshDataMap)
			{
				std::string _meshName = StringHelper::WStringToString(_meshPair.second._meshName)/* + "_" + name*/;

				BinaryData::MeshData meshData;
				if (prefabData->_boneDatas.size() == 0)
				{
					meshData = { _meshName, _meshPair.second._vertexAttributes, _meshPair.second._indexAttributes, _meshPair.second._boundingMinBox, _meshPair.second._boundingMaxBox };
				}
				else
				{
					meshData = { _meshName, _meshPair.second._vertexAttributes, _meshPair.second._indexAttributes, _meshPair.second._boundingMinBox, _meshPair.second._boundingMaxBox, fileName};
				}

				std::ofstream mesh_ofs(path + meshPathStr + _meshName + ".mesh", std::ios_base::binary);
				boost::iostreams::filtering_stream<boost::iostreams::output> meshBuffer;
				meshBuffer.push(boost::iostreams::zlib_compressor());
				meshBuffer.push(mesh_ofs);

				boost::archive::binary_oarchive oaMesh(meshBuffer);
				oaMesh << meshData;

				meshInfoList.emplace_back(meshData.meshName);
			}

			// Animation Serialize
			std::vector<std::string> animationClipList;

			std::vector<BinaryData::AnimationClipData> _animClipDatas;

			for (auto& _animData : prefabData->_animationClipDatas)
			{
				BinaryData::AnimationClipData _animationClipData;

				for (auto& _snap : _animData._snapList)
				{
					BinaryData::AnimationSnapData _snapData;

					_snapData._nodeName = StringHelper::WStringToString(_snap._nodeName);
					_snapData._parentNodeName = StringHelper::WStringToString(_snap._parentNodeName);

					_snapData._keyFrameList = _snap._keyFrameList;

					//for (auto& _key : _snap._keyFrameList)
					//{
					//	BinaryData::KeyFrame _keyFrame{ _key._time, _key._pos, _key._rot, _key._scale };

					//	_snapData._keyFrameList.emplace_back(_keyFrame);
					//}

					_animationClipData._snapList.emplace_back(_snapData);
				}

				_animationClipData._clipName = StringHelper::WStringToString(_animData._clipName);

				_animationClipData._frameRate = _animData._frameRate;
				_animationClipData._tickPerFrame = _animData._tickPerFrame;
				_animationClipData._totalKeyFrame = _animData._totalKeyFrame;
				_animationClipData._startKeyFrame = _animData._startKeyFrame;
				_animationClipData._endKeyFrame = _animData._endKeyFrame;

				ReconstructionAnimName(_animationClipData._clipName);
				std::ofstream anim_ofs(path + animationPathStr + _animationClipData._clipName + ".anim", std::ios_base::binary);
				boost::iostreams::filtering_stream<boost::iostreams::output> animBuffer;
				animBuffer.push(boost::iostreams::zlib_compressor());
				animBuffer.push(anim_ofs);

				boost::archive::binary_oarchive oaAnim(animBuffer);
				oaAnim << _animationClipData;

				//animationClipList.emplace_back(animData.animationName);
			}

			//BinaryData::ModelData modelData(meshInfoList, materialList, animationClipList, boneInfoList, prefabData->isSkinnedAnimation);

			//std::ofstream model_ofs(path + modelPathStr + name + ".model", std::ios_base::binary);
			//boost::iostreams::filtering_stream<boost::iostreams::output> modelBuffer;
			//modelBuffer.push(boost::iostreams::zlib_compressor());
			//modelBuffer.push(model_ofs);
			//boost::archive::binary_oarchive oaModel(modelBuffer);	// 모델에는 Mesh, Material, Animation들의 이름만 담길것

			//oaModel << modelData;
		}
		catch (const boost::archive::archive_exception e) {
			std::cout << "boost::archive exception code is " << e.code << std::endl;
		}
	}

	void BinarySerializer::ReconstructionAnimName(std::string& name)
	{
		if (name.find("|") != std::string::npos)
		{
			size_t idx = name.find("|") + 1;

			name.erase(0, idx);

			ReconstructionAnimName(name);
		}
	}
}