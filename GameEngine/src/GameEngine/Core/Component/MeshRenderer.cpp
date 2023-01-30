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
			m_MeshFilter = GetComponent<MeshFilter>();
		}

		void MeshRenderer::Render()
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

						_renderObject.m_UpdateResourcePerDraw._dataSrc = &_perObject;
						_renderObject.m_UpdateResourcePerDraw._datasize = sizeof(Math::Matrix) * 2;

						if (m_Materials[i]->GetAlbedoTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 4;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetAlbedoTexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						if (m_Materials[i]->GetNormalTexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 5;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetNormalTexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						if (m_Materials[i]->GetMRATexture() != nullptr)
						{
							Graphics::UpdateResourceData _data;

							_data._resourceType = Graphics::ResourceType::Texture;

							_data._index = 6;

							_data._dataSrc = reinterpret_cast<void*>(m_Materials[i]->GetMRATexture());

							_renderObject.m_UpdateResources.push_back(_data);
						}

						_materialBuffer->RegistRenderObject(_renderObject);
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