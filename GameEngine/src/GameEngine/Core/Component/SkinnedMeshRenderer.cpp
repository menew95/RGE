#include "GameEngine/GameEnginePCH.h"

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
		metadata(GameEngine::Core::MetaDataType::UUID, "GetUUID")
	)
	.property("m_RootBone", &GameEngine::Core::SkinnedMeshRenderer::GetRootBone, &GameEngine::Core::SkinnedMeshRenderer::SetRootBone)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckRootVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetUUID")
	)
	.method("GetUUID", &GameEngine::Core::Material::GetUUID)
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
			m_MeshFilter = GetComponent<MeshFilter>();
		}

		void SkinnedMeshRenderer::Render()
		{
			if (m_MeshFilter.expired() == false)
			{
				std::shared_ptr<Mesh>& _sharedMesh = m_MeshFilter.lock()->GetSharedMesh();

				if (_sharedMesh != nullptr)
				{
					uint32 _subMeshCnt = _sharedMesh->GetSubMeshCount();

					for (uint32 i = 0; i < _subMeshCnt && i < static_cast<uint32>(m_Materials.size()); i++)
					{
						if (m_Materials[i] == nullptr) continue;

						Graphics::RenderObject _renderObject;

						auto* _materialBuffer = m_Materials[i]->GetMaterialBuffer();

						_renderObject.m_MeshBuffer = _sharedMesh->GetMeshBuffer();
						_renderObject.m_MaterialBuffer = _materialBuffer;

						_perObject._world = GetTransform()->GetWorldTM();
						_perObject._worldInvTranspose = GetTransform()->GetWorldTM().Invert().Transpose();

						Graphics::UpdateResourceData _perObjectResource
						{
							_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
							_perObjectResource._index = 1,
							_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
							_perObjectResource._dataSrc = &_perObject,
							_perObjectResource._datasize = sizeof(Math::Matrix) * 2
						};

						_renderObject.m_UpdateResourcePerObjects.push_back(_perObjectResource);

						UpdateBoneTransform();

						Graphics::UpdateResourceData _perSkinnedObjectResource
						{
							_perSkinnedObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
							_perSkinnedObjectResource._index = 2,
							_perSkinnedObjectResource._resourceType = Graphics::ResourceType::Buffer,
							_perSkinnedObjectResource._dataSrc = &_perSkinnedObject,
							_perSkinnedObjectResource._datasize = sizeof(Math::Matrix) * 128
						};

						_renderObject.m_UpdateResourcePerObjects.push_back(_perSkinnedObjectResource);

						if (m_Materials[i]->GetAlbedoTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 5;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetAlbedoTexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						if (m_Materials[i]->GetNormalTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 6;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetNormalTexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						if (m_Materials[i]->GetMRATexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 7;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetMRATexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						// Todo : 임시 나중에 그래픽스 시스템이 랜더 패스 소유하고 리스트를 순회 할 때 바꿀것
						GraphicsSystem::GetInstance()->RegistRenderObject(1, _renderObject);

						//_materialBuffer->RegistRenderObject(_renderObject);
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