#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/GameObject.h"
#include "GameEngine/Core/System/Resources.h"
#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"
#include "GameEngine/Core/Resource/AnimationClip.h"

#include "GameEngine/Core/Resource/Prefab.h"

#include "GameEngine/Core/Component/Transform.h"
#include "GameEngine/Core/Component/MeshFilter.h"
#include "GameEngine/Core/Component/MeshRenderer.h"
#include "GameEngine/Core/Component/SkinnedMeshRenderer.h"

#include "GameEngine/Core/Component/Animation.h"

#include "Importer/Importer.h"
#include "Importer/ImportInfo.h"

#include "Struct/VertexAttribute.h"

#include "GameEngine/Utility/Serialize/Serialization.h"

void ReconstructionFileName(tstring& name)
{
	if (name.find(L"/") != tstring::npos)
	{
		size_t idx = name.find(L"/") + 1;

		name.erase(0, idx);

		ReconstructionFileName(name);
	}
}

void RemoveExtension(tstring& name)
{
	size_t idx = name.find(L".") + 1;

	size_t dot_pos = name.find_last_of(L".");
	if (dot_pos != std::wstring::npos)
	{
		name = name.substr(0, dot_pos);
	}
}

namespace GameEngine 
{
	namespace Core
	{
		DEFINE_SINGLETON_CLASS(Resources, {}, {})

		void Resources::Init()
		{
			{
				std::vector<Common::VertexAttribute> _vertexAttributes =
				{
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +0.f, -1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +0.f, +1.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, +1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +0.f, -1.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { -1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { -1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +1.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, -1.f }, { 0, 0, 0, 0 }, { +0.f, +0.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, +1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +0.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
					{ { +1.f, -1.f, +1.f }, { 0, 0, 0, 0 }, { +1.f, +1.f }, { +1.f, +0.f, +0.f }, { 0.f, 0.f, 0.f }, { 0, 0, 0, 0 }, { 0.f, 0.f, 0.f, 0.f } },
				};

				std::vector<std::vector<uint32>> _indexAttributes = {
					{
						0, 1, 2,
						0, 2, 3,
						4, 5, 6,
						4, 6, 7,
						8, 9, 10,
						8, 10, 11,
						12, 13, 14,
						12, 14, 15,
						16, 17, 18,
						16, 18, 19,
						20, 21, 22,
						20, 22, 23
					}
				};
				std::shared_ptr<Mesh> _newMesh = std::make_shared<Mesh>(_vertexAttributes, _indexAttributes);

				_newMesh->SetName(TEXT("Box"));

				GraphicsSystem::GetInstance()->CreateMeshBuffer(_newMesh);

				m_MeshMap.insert(std::make_pair(TEXT("Box"), _newMesh));
			}
			
			//LoadFBX(TEXT("Asset/FBX/Joy.fbx"));
			//LoadFBX(TEXT("Asset/FBX/Wooden_Crate.fbx"));
			//LoadFBX(TEXT("Asset/FBX/Wooden_Crate_Normal_MRA.fbx"));
		}

		void* Resources::Load(const tstring& filePath)
		{
			// Todo : 리소스 로드를 템플릿 함수로 만들지 정해야함
			return nullptr;
		}

		void Resources::LoadFBX(const tstring& filePath)
		{
			using namespace Utility;

			PrefabData _newPrefabData;

			Importer _importer;

			_importer.LoadPrefabDataFormFile(filePath, _newPrefabData);

			for (auto& _pair : _newPrefabData._meshDataMap)
			{
				std::shared_ptr<Mesh> _newMesh = std::make_shared<Mesh>(_pair.second._vertexAttributes, _pair.second._indexAttributes);

				_newMesh->SetName(_pair.first);

				GraphicsSystem::GetInstance()->CreateMeshBuffer(_newMesh);

				m_MeshMap.insert(std::make_pair(_pair.first, _newMesh));
			}

			for (auto& _pair : _newPrefabData._materialDataMap)
			{
				std::shared_ptr<Material> _newMaterial = std::make_shared<Material>();

				_newMaterial->SetName(_pair.first);


				//_pair.second._albedoMapTexture;
				//_pair.second._normalMapTexture;
				//_pair.second._metalicRoughnessMapTexture;
				_pair.second._emissiveMapTexture;
				_pair.second._ambientMapTexture;
				_pair.second._specularMapTexture;

				GraphicsSystem::GetInstance()->CreateMaterialBuffer(_newMaterial);

				if(_pair.second._albedoMapTexture.length() > 0) _newMaterial->SetAlbedoTexture(GraphicsSystem::GetInstance()->LoadTexture(_pair.second._albedoMapTexture));
				if(_pair.second._normalMapTexture.length() > 0) _newMaterial->SetNormalTexture(GraphicsSystem::GetInstance()->LoadTexture(_pair.second._normalMapTexture));
				if(_pair.second._metalicRoughnessMapTexture.length() > 0) _newMaterial->SetMRATexture(GraphicsSystem::GetInstance()->LoadTexture(_pair.second._metalicRoughnessMapTexture));

				m_MaterialMap.insert(std::make_pair(_pair.first, _newMaterial));
			}

			for (auto& _animData : _newPrefabData._animationClipDatas)
			{
				CreateAnimationClip(&_animData);
			}

			CreatePrefab(_newPrefabData);
		}

		void Resources::CreatePrefab(const Utility::PrefabData& prefabData)
		{
			auto _rootGameObject = GameObject::Instantiate();

			tstring _fileName = prefabData._filePath;

			RemoveExtension(_fileName);

			ReconstructionFileName(_fileName);

			_rootGameObject->SetName(_fileName);

			std::unordered_map<tstring, std::shared_ptr<GameObject>> _newGameObjectMap;
			std::vector<std::shared_ptr<GameObject>> _newGameObjectList;

			std::unordered_map<tstring, std::shared_ptr<GameObject>> _newBoneMap;
			std::vector<std::shared_ptr<GameObject>> _newBoneList;

			_newGameObjectMap.insert(std::make_pair(TEXT("RootNode"), _rootGameObject));

			_newGameObjectList.reserve(prefabData._gameObjectDatas.size());

#pragma region 빈 게임 오브젝트 생성
			for (size_t i = 0; i < prefabData._gameObjectDatas.size(); i++)
			{
				std::shared_ptr<GameObject> _newGameObject = GameObject::Instantiate();

				_newGameObject->SetName(prefabData._gameObjectDatas[i]._gameObjectName);
				_newGameObjectList.push_back(_newGameObject);
				_newGameObjectMap.insert(std::make_pair(prefabData._gameObjectDatas[i]._gameObjectName, _newGameObject));

#pragma region 부모 자식 관계 및 트랜스폼 설정

				auto _trans = _newGameObject->GetTransform();

				if (prefabData._gameObjectDatas[i]._hasParent)
				{
					auto _iter = _newGameObjectMap.find(prefabData._gameObjectDatas[i]._parent);

					assert(_iter != _newGameObjectMap.end());

					auto _findParent = _iter->second->GetTransform();

					_trans->SetParent(_findParent);
				}
				else
				{
					auto _root = _rootGameObject->GetTransform();

					_trans->SetParent(_root);
				}

				//_trans->SetLocal(prefabData._gameObjectDatas[i]._localTM);
				_trans->SetWorldTM(prefabData._gameObjectDatas[i]._worldTM);
#pragma endregion
			}
#pragma endregion

#pragma region Skinning
			if (prefabData._boneDatas.size() > 0)
			{
				_newBoneList.reserve(prefabData._boneDatas.size());

				for (size_t i = 0; i < prefabData._boneDatas.size(); i++)
				{
					std::shared_ptr<GameObject> _newBone = GameObject::Instantiate();

					_newBone->SetName(prefabData._boneDatas[i]._boneName);

					
#if defined(_DEBUG) || defined(DEBUG)
					//auto _material = GetMaterial(TEXT("Material.001"));
					//auto _mesh = GetMesh(TEXT("Box"));
					//
					//_newBone->AddComponent<MeshRenderer>()->AddMaterial(_material);
					//_newBone->AddComponent<MeshFilter>()->SetSharedMesh(_mesh);
#endif // defined(_DEBUG) || defined(DEBUG)


					_newBoneMap.insert(std::make_pair(prefabData._boneDatas[i]._boneName, _newBone));

					_newBoneList.push_back(_newBone);

#pragma region Bone 부모 자식 관계 설정 및 로컬 트랜스폼 설정
					auto _boneTrans = _newBone->GetTransform();

					if (prefabData._boneDatas[i]._parentBoneIndex != -1)
					{
						assert(_newBoneList[prefabData._boneDatas[i]._parentBoneIndex]->GetName() == prefabData._boneDatas[i]._parentBoneName);

						_boneTrans->SetParent(_newBoneList[prefabData._boneDatas[i]._parentBoneIndex]->GetTransform());
					}
					else
					{
						auto _iter = _newGameObjectMap.find(prefabData._boneDatas[i]._parentBoneName);

						assert(_iter != _newGameObjectMap.end());

						_boneTrans->SetParent(_iter->second->GetTransform());
					}

					//_boneTrans->SetLocal(prefabData._boneDatas[i]._localTM);
					_boneTrans->SetWorldTM(prefabData._boneDatas[i]._worldTM);
				}
#pragma endregion
			}
#pragma endregion

#pragma region 게임 오브젝트 생성
			for (size_t i = 0; i < prefabData._gameObjectDatas.size(); i++)
			{
				std::shared_ptr<GameObject> _newGameObject = _newGameObjectList[i];

				_newGameObject->SetName(prefabData._gameObjectDatas[i]._gameObjectName);

				if (prefabData._gameObjectDatas[i]._hasMesh)
				{
					auto _filterCom = _newGameObject->AddComponent<MeshFilter>();

					auto _findMesh = GetMesh(prefabData._gameObjectDatas[i]._mesh);

					assert(prefabData._meshDataMap.find(prefabData._gameObjectDatas[i]._mesh) != prefabData._meshDataMap.end());

					auto _findMeshData = prefabData._meshDataMap.at(prefabData._gameObjectDatas[i]._mesh);

					_filterCom->SetSharedMesh(_findMesh);

					if (_findMeshData._isSkin == false)
					{
						auto _meshRenCom = _newGameObject->AddComponent<MeshRenderer>();

						for (auto&_matName : _findMeshData._materials)
						{
							auto _findMat = GetMaterial(_matName);

							_meshRenCom->AddMaterial(_findMat);
						}
					}
					else
					{
						auto _skinMeshRenCom = _newGameObject->AddComponent<SkinnedMeshRenderer>();

						_skinMeshRenCom->SetRootBone(_newBoneList[0]->GetTransform());

						for (auto& _matName : _findMeshData._materials)
						{
							auto _findMat = GetMaterial(_matName);

							_skinMeshRenCom->AddMaterial(_findMat);
						}

						for (uint32 _boneIndex = 0; _boneIndex < _newBoneList.size(); _boneIndex++)
						{
							_skinMeshRenCom->AddBone(_newBoneList[_boneIndex]->GetTransform(), prefabData._boneDatas[_boneIndex]._offsetTM);
						}
					}
				}
			}
#pragma endregion

			if (prefabData._animationClipDatas.size() > 0)
			{
				auto _animCom = _rootGameObject->AddComponent<Animation>();

				for (size_t i = 0; i < prefabData._animationClipDatas.size(); i++)
				{
					auto _clip = GetAnimationClip(prefabData._animationClipDatas[i]._clipName);

					_animCom->AddClip(_clip);
				}
			}

			uuid _uuid = prefabData._filePath;
			std::shared_ptr<Prefab> _newPrefab = std::make_shared<Prefab>();

			_newPrefab->SetRootGameObject(_rootGameObject);
			
			_newPrefab->SetName(_fileName);
			Serializable _ser;

			_ser.serialize(*_newPrefab);

			m_PrefabMap.insert(std::make_pair(_fileName, _newPrefab));
		}

		template<>
		std::shared_ptr<Mesh> Resources::GetResource(uuid uuid)
		{
			auto _find = m_MeshMap.find(uuid);

			if (_find != m_MeshMap.end())
				return _find->second;
				
			if (LoadMesh(uuid))
			{
				return GetResource<Mesh>(uuid);
			}

			assert(false);
			return std::shared_ptr<Mesh>(nullptr);
		}

		template<>
		std::shared_ptr<SkinnedData> Resources::GetResource(uuid uuid)
		{
			auto _find = m_SkinnedMap.find(uuid);

			if (_find != m_SkinnedMap.end())
				return _find->second;

			if (LoadSkinned(uuid))
			{
				return GetResource<SkinnedData>(uuid);
			}

			assert(false);
			return std::shared_ptr<SkinnedData>(nullptr);
		}

		template<>
		std::shared_ptr<Material> Resources::GetResource(uuid uuid)
		{
			auto _find = m_MaterialMap.find(uuid);

			if (_find != m_MaterialMap.end())
				return _find->second;

			if (LoadMaterial(uuid))
			{
				return GetResource<Material>(uuid);
			}

			assert(false);
			return std::shared_ptr<Material>(nullptr);
		}

		template<>
		std::shared_ptr<AnimationClip> Resources::GetResource(uuid uuid)
		{
			auto _find = m_AnimationClipMap.find(uuid);

			if (_find != m_AnimationClipMap.end())
				return _find->second;

			if (LoadAnimation(uuid))
			{
				return GetResource<AnimationClip>(uuid);
			}

			assert(false);
			return std::shared_ptr<AnimationClip>(nullptr);
		}

		template<>
		std::shared_ptr<Prefab> Resources::GetResource(uuid uuid)
		{
			auto _find = m_PrefabMap.find(uuid);

			if (_find != m_PrefabMap.end())
				return _find->second;

			if (LoadPrefab(uuid))
			{
				return GetResource<Prefab>(uuid);
			}

			assert(false);
			return std::shared_ptr<Prefab>(nullptr);
		}
		
		std::shared_ptr<GameEngine::Core::Mesh>& Resources::GetMesh(uuid uuid)
		{
			auto _find = m_MeshMap.find(uuid);

			assert(_find != m_MeshMap.end());

			return _find->second;
		}

		std::shared_ptr<GameEngine::Core::Material>& Resources::GetMaterial(uuid uuid)
		{
			auto _find = m_MaterialMap.find(uuid);

			assert(_find != m_MaterialMap.end());
			
			return _find->second;
		}

		std::shared_ptr<GameEngine::Core::AnimationClip>& Resources::GetAnimationClip(uuid uuid)
		{
			auto _find = m_AnimationClipMap.find(uuid);

			assert(_find != m_AnimationClipMap.end());

			return _find->second;
		}

		std::shared_ptr<GameEngine::Core::Prefab>& Resources::GetPrefab(uuid uuid)
		{
			auto _find = m_PrefabMap.find(uuid);

			assert(_find != m_PrefabMap.end());

			return _find->second;
		}

		bool Resources::LoadMesh(uuid uuid)
		{
			using namespace Utility;

			Importer _importer;

			Utility::MeshData _meshData;

			if (!_importer.Deserialize(_meshData, uuid))
				return false;

			std::shared_ptr<Mesh> _newMesh = std::make_shared<Mesh>(_meshData._vertexAttributes, _meshData._indexAttributes);

			_newMesh->SetName(_meshData._meshName);
			_newMesh->SetUUID(_meshData._meshName);

			if (_meshData._isSkin)
			{
				_newMesh->SetBoneName(_meshData._skinName);

				auto _skin = GetResource<SkinnedData>(_meshData._skinName);

				_newMesh->SetSkinned(_skin);
			}

			GraphicsSystem::GetInstance()->CreateMeshBuffer(_newMesh);

			m_MeshMap.insert(std::make_pair(uuid, _newMesh));

			return true;
		}

		bool Resources::LoadSkinned(uuid uuid)
		{
			using namespace Utility;

			Importer _importer;

			std::vector<Utility::BoneData> _newBones;

			if (!_importer.Deserialize(_newBones, uuid))
				return false;

			std::shared_ptr<SkinnedData> _newSkinned = std::make_shared<SkinnedData>();

			for (auto _boneData : _newBones)
			{
				_newSkinned->_boneDatas.push_back({ _boneData._boneName, _boneData._offsetTM });
			}

			m_SkinnedMap.insert(std::make_pair(uuid, _newSkinned));

			return true;
		}

		bool Resources::LoadMaterial(uuid uuid)
		{
			using namespace Utility;

			Importer _importer;

			Utility::MaterialData _matData;

			if (!_importer.Deserialize(_matData, uuid))
				return false;

			std::shared_ptr<Material> _newMaterial = std::make_shared<Material>();

			_newMaterial->SetName(_matData._matarialName);
			_newMaterial->SetUUID(_matData._matarialName);

			GraphicsSystem::GetInstance()->CreateMaterialBuffer(_newMaterial);

			if (_matData._albedoMapTexture.length() > 0)
				_newMaterial->SetAlbedoTexture(GraphicsSystem::GetInstance()->LoadTexture(_matData._albedoMapTexture));

			if (_matData._normalMapTexture.length() > 0)
				_newMaterial->SetNormalTexture(GraphicsSystem::GetInstance()->LoadTexture(_matData._normalMapTexture));

			if (_matData._metalicRoughnessMapTexture.length() > 0)
				_newMaterial->SetMRATexture(GraphicsSystem::GetInstance()->LoadTexture(_matData._metalicRoughnessMapTexture));

			m_MaterialMap.insert(std::make_pair(uuid, _newMaterial));

			return true;
		}

		bool Resources::LoadAnimation(uuid uuid)
		{
			using namespace Utility;

			Importer _importer;

			Utility::AnimationClipData _clipData;

			if (!_importer.Deserialize(_clipData, uuid))
				return false;
			
			CreateAnimationClip(&_clipData);

			return true;
		}

		bool Resources::LoadPrefab(uuid uuid)
		{
			std::shared_ptr<Prefab> _newPrefab = std::make_shared<Prefab>();

			Serializable _ser;

			if (!_ser.Deserialize(*_newPrefab, uuid))
				return false;

			_newPrefab->SetName(uuid);
			_newPrefab->SetUUID(uuid);
			
			m_PrefabMap.insert(std::make_pair(uuid, _newPrefab));

			return true;
		}

		void Resources::CreateAnimationClip(Utility::AnimationClipData* clipData)
		{
			std::shared_ptr<AnimationClip> _newAnimClip = std::make_shared<AnimationClip>();

			_newAnimClip->SetClipName(clipData->_clipName);
			_newAnimClip->SetName(clipData->_clipName);
			_newAnimClip->SetUUID(clipData->_clipName);

			float _totalTime = (float)(clipData->_totalKeyFrame - 1) / clipData->_frameRate;

			for (size_t _snpIdx = 0; _snpIdx < clipData->_snapList.size(); _snpIdx++)
			{
				AnimationSnap _newSnap;

				_newSnap.m_TargetName = clipData->_snapList[_snpIdx]._nodeName;
				_newSnap.m_MaxFrameRate = _totalTime;

				for (size_t _frameIdx = 0; _frameIdx < clipData->_snapList[_snpIdx]._keyFrameList.size(); _frameIdx++)
				{
					KeyFrame _pos
					{ 
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._time,
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._pos
					};

					_newSnap.AddKeyFrame(_pos, AnimationSnap::Position);

					KeyFrame _rot
					{
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._time,
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._rot
					};

					_newSnap.AddKeyFrame(_rot, AnimationSnap::Rotate);

					KeyFrame _scale
					{
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._time,
						clipData->_snapList[_snpIdx]._keyFrameList[_frameIdx]._scale
					};

					_newSnap.AddKeyFrame(_scale, AnimationSnap::Scale);
				}

				_newAnimClip->AddAniamtionSnap(_newSnap);
			}

			m_AnimationClipMap.insert(std::make_pair(clipData->_clipName, _newAnimClip));
		}
	}
}