#include "GameEngine/GameEnginePCH.h"
#include "GameEngine/Core/Component/MeshRenderer.h"
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
	rttr::registration::class_<GameEngine::Core::MeshRenderer>("MeshRenderer")
	.constructor<std::shared_ptr<GameEngine::Core::GameObject>&, const tstring&>()
	.property("m_Materials", &GameEngine::Core::MeshRenderer::GetMaterials, &GameEngine::Core::MeshRenderer::SetMaterials)
	(
		metadata(GameEngine::Core::MetaData::Serializable, GameEngine::Core::MetaDataType::UUID),
		metadata(GameEngine::Core::Util::Check_Vaild, "CheckVaild"),
		metadata(GameEngine::Core::MetaDataType::UUID, "GetName"),
		metadata(GameEngine::Core::MetaData::ObjectType, GameEngine::Core::ObjectType::Resource),
		metadata(GameEngine::Core::ObjectType::Resource, GameEngine::Core::ResourceType::Material)
	)
	.method("GetName", &GameEngine::Core::Material::GetName)
	.method("CheckVaild", &GameEngine::Core::MeshRenderer::CheckVaild);
}

namespace GameEngine
{
	namespace Core
	{
		MeshRenderer::MeshRenderer(std::shared_ptr<GameObject>& gameObject, const tstring& componentName /*= TEXT("MeshRenderer")*/)
			: Renderer(gameObject, componentName)
		{

		}

		MeshRenderer::~MeshRenderer()
		{

		}

		void MeshRenderer::Awake()
		{
			__super::Awake();
			
			m_MeshFilter = GetComponent<MeshFilter>();

			Graphics::UpdateResourceData _perObjectResource
			{
				_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
				_perObjectResource._index = 1,
				_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
				_perObjectResource._dataSrc = &m_RenderObject->m_TransformMatrix,
				_perObjectResource._datasize = sizeof(Math::Matrix) * 2
			};

			m_RenderObject->m_UpdateResourcePerObjects.emplace_back(_perObjectResource);
		}

		void MeshRenderer::Render()
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

					for (uint32 i = 0; i < _subMeshCnt && i < static_cast<uint32>(m_Materials.size()); i++)
					{
						if (m_Materials[i] == nullptr) m_RenderObject->m_MaterialBuffers.push_back(nullptr);
						{
							m_RenderObject->m_MaterialBuffers.push_back(nullptr);
							continue;
						}

						auto* _materialBuffer = m_Materials[i]->GetMaterialBuffer();

						m_RenderObject->m_MaterialBuffers.push_back(_materialBuffer);

						//_perObject._world = GetTransform()->GetWorldTM();
						//_perObject._worldInvTranspose = GetTransform()->GetWorldTM().Invert().Transpose();

						/*Graphics::UpdateResourceData _perObjectResource
						{
							_perObjectResource._updateTime = Graphics::eUpdateTime::PerObject,
							_perObjectResource._index = 1,
							_perObjectResource._resourceType = Graphics::ResourceType::Buffer,
							_perObjectResource._dataSrc = &_perObject,
							_perObjectResource._datasize = sizeof(Math::Matrix) * 2
						};

						_renderObject.m_UpdateResourcePerObjects.emplace_back(_perObjectResource);*/

						/*uint32 _idx = 0;

						if (m_Materials[i]->GetAlbedoTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 4;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetAlbedoTexture());

							_renderObject.m_UpdateResources.emplace_back(_data);

							_idx++;
						}

						if (m_Materials[i]->GetNormalTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 5;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetNormalTexture());

							_renderObject.m_UpdateResources.emplace_back(_data);

							_idx++;
						}

						if (m_Materials[i]->GetMRATexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 6;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetMRATexture());

							_renderObject.m_UpdateResources.emplace_back(_data);

							_idx++;
						}*/

						//Graphics::UpdateResourceData _perMaterialResource
						//{
						//	_perMaterialResource._updateTime = Graphics::eUpdateTime::PerMaterial,
						//	_perMaterialResource._index = 2,
						//	_perMaterialResource._resourceType = Graphics::ResourceType::Buffer,
						//	_perMaterialResource._dataSrc = m_Materials[i]->GetStandardResource(),
						//	_perMaterialResource._datasize = 48 // Todo : 나중에 바꾸자
						//};

						//_renderObject.m_UpdateResources.push_back(_perMaterialResource);

						//_renderObject.m_RenderPassIdx = _idx;
						//GraphicsSystem::GetInstance()->RegistRenderObject(_idx, _renderObject);

						//if (m_bIsShadowCasting)
						//{
						//	Graphics::RenderObject _shadow;
						//	_shadow.m_World = GetTransform()->GetWorldTM();
						//	_shadow.m_MeshBuffer = _sharedMesh->GetMeshBuffer();
						//	_shadow.m_MaterialBuffer = _materialBuffer;

						//	//_perObjectResource._index = 0;
						//	_shadow.m_UpdateResourcePerObjects.emplace_back(_perObjectResource);

						//	GraphicsSystem::GetInstance()->RegistShadowObject(0, _shadow);
						//}
					}
				
				}
			}
		}

		void MeshRenderer::Link()
		{
			m_MeshFilter = GetComponent<MeshFilter>();
		}

		void MeshRenderer::Unlink()
		{
			m_MeshFilter.reset();
		}

	}
}