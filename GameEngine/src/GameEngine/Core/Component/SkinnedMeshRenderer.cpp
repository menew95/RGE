﻿#include "GameEngine/GameEnginePCH.h"

#include "GameEngine/Core/Component/SkinnedMeshRenderer.h"
#include "GameEngine/Core/Component/MeshFilter.h"
#include "GameEngine/Core/Component/Transform.h"

#include "GameEngine/Core/System/GraphicsSystem.h"

#include "GameEngine/Core/Resource/Mesh.h"
#include "GameEngine/Core/Resource/Material.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

#include <rttr/registration.h>
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameEngine::Core::SkinnedMeshRenderer>("SkinnedMeshRenderer")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_Materials", &GameEngine::Core::SkinnedMeshRenderer::GetMaterials, &GameEngine::Core::SkinnedMeshRenderer::SetMaterials)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Resource),
		metadata(GameEngine::Core::ObjectType::Resource, GameEngine::Core::ResourceType::Material)
	)
	.property("m_RootBone", &GameEngine::Core::SkinnedMeshRenderer::GetRootBone, &GameEngine::Core::SkinnedMeshRenderer::SetRootBone)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckRootVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetGameObjectName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Component),
		metadata(GameEngine::Core::ObjectType::Component, "Transform")
	)
	.method("GetName", &GameEngine::Core::Material::GetName)
	.method("CheckVaild", &GameEngine::Core::SkinnedMeshRenderer::CheckVaild)
	.method("CheckRootVaild", &GameEngine::Core::SkinnedMeshRenderer::CheckRootVaild);
}


namespace GameEngine
{
	namespace Core
	{

		SkinnedMeshRenderer::SkinnedMeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("MeshRenderer")*/)
			: Renderer(gameObject, componentName)
		{

		}

		SkinnedMeshRenderer::~SkinnedMeshRenderer()
		{

		}

		void SkinnedMeshRenderer::Awake()
		{
			__super::Awake();

			m_MeshFilter = GetComponent<MeshFilter>();

			if (!m_MeshFilter.expired() && m_MeshFilter.lock() != nullptr)
			{
				auto _sharedMesh = m_MeshFilter.lock()->GetSharedMesh();

				if (_sharedMesh->IsSkinned() && m_RootBone != nullptr)
				{
					auto _skinnedData = _sharedMesh->GetSkinned();

					auto _rootGO = m_RootBone->GetGameObject();
					for (auto& _bone : _skinnedData->_boneDatas)
					{
						auto* _boneGO = _rootGO->FindGameObject(_bone._boneName);
						if (_boneGO != nullptr)
						{
							AddBone(_boneGO->GetTransform(), _bone._offsetTM);
						}
					}
				}
			}

			Graphics::UpdateResourceData _perObjectResource
			{
				_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
				_perObjectResource._index = 1,
				_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
				_perObjectResource._dataSrc = &m_RenderObject->m_TransformMatrix,
				_perObjectResource._datasize = sizeof(Math::Matrix) * 2
			};

			m_RenderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

			Graphics::UpdateResourceData _perSkinnedObjectResource
			{
				_perSkinnedObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
				_perSkinnedObjectResource._index = 2,
				_perSkinnedObjectResource._resourceType = Graphics::ResourceType::Buffer,
				_perSkinnedObjectResource._dataSrc = m_RenderObject->m_pSkinnedData,
				_perSkinnedObjectResource._datasize = sizeof(Math::Matrix) * 128
			};

			m_RenderObject->m_UpdateResourcePerObjects.emplace_back(_perSkinnedObjectResource);
		}

		void SkinnedMeshRenderer::Render()
		{
			if (m_MeshFilter.expired() == false)
			{
				std::shared_ptr<Mesh>& _sharedMesh = m_MeshFilter.lock()->GetSharedMesh();

				if (_sharedMesh != nullptr)
				{
					uint32 _subMeshCnt = _sharedMesh->GetSubMeshCount();

					m_RenderObject->m_TransformMatrix._world = GetTransform()->GetWorldTM();
					m_RenderObject->m_TransformMatrix._worldInv = GetTransform()->GetWorldTM().Invert().Transpose();

					m_RenderObject->m_bIsCastShadow = m_bIsShadowCasting;

					m_RenderObject->m_MeshBuffer = _sharedMesh->GetMeshBuffer();

					m_RenderObject->m_MaterialBuffers.clear();

					UpdateBoneTransform();

					m_RenderObject->m_bIsSkinned = true;
					m_RenderObject->m_pSkinnedData = &_perSkinnedObject;

					for (uint32 i = 0; i < _subMeshCnt && i < static_cast<uint32>(m_Materials.size()); i++)
					{
						if (m_Materials[i] == nullptr) m_RenderObject->m_MaterialBuffers.push_back(nullptr);
						{
							m_RenderObject->m_MaterialBuffers.push_back(nullptr);
							continue;
						}

						auto* _materialBuffer = m_Materials[i]->GetMaterialBuffer();

						m_RenderObject->m_MaterialBuffers.push_back(_materialBuffer);

					}
				}
			}
		}

		void SkinnedMeshRenderer::Link()
		{
			m_MeshFilter = GetComponent<MeshFilter>();
		}

		void SkinnedMeshRenderer::Unlink()
		{
			m_MeshFilter.reset();
		}

		void SkinnedMeshRenderer::UpdateBoneTransform()
		{
			for (uint32 i = 0; i < m_BoneTransformList.size(); i++)
			{
				if(m_BoneTransformList[i].expired())
					continue;

				auto _world = m_BoneTransformList[i].lock()->GetWorldTM();
				auto _local = m_BoneTransformList[i].lock()->GetLocalTM();

				_perSkinnedObject._boneTransform[i] = m_BoneOffset[i] * m_BoneTransformList[i].lock()->GetWorldTM();
			}
		}
	}
}