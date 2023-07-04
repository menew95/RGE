#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/InstanceBuffer.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"


#include "GraphicsModule/Core/CommandBuffer.h"

namespace Graphics
{
	InstanceRenderer::InstanceRenderer(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
		, m_CommandBuffer(commandBuffer)
	{
		Initialize();
	}

	InstanceRenderer::~InstanceRenderer()
	{

	}

	void InstanceRenderer::RegistInstanceQueue(const RenderData& renderData)
	{
		if (renderData._materialIdx != -1 && renderData._renderObject->m_MaterialBuffers.size() < renderData._materialIdx)
		{
			MaterialBuffer* _matBuffer = renderData._renderObject->m_MaterialBuffers[renderData._materialIdx];

			auto& _uuid = _matBuffer->GetUUID();

			for (uint32 i = 0; i < m_InstanceQueue[_uuid]._meshInstanceDatas.size(); i++)
			{
				auto& _materialInstance = m_InstanceQueue[_uuid];

				if (_materialInstance._meshInstanceDatas[i]._meshBuffer->GetUUID()
					== renderData._renderObject->m_MeshBuffer->GetUUID()
					&& _materialInstance._meshInstanceDatas[i]._subMeshIdx == renderData._subMeshIdx)
				{
					_materialInstance._meshInstanceDatas[i]._renderObjects.emplace_back(renderData);

					return;
				}
			}

			MeshInstanceData _meshInstanceData;

			_meshInstanceData._meshBuffer = renderData._renderObject->m_MeshBuffer;

			_meshInstanceData._bIsSkin = renderData._renderObject->m_bIsSkinned;

			_meshInstanceData._subMeshIdx = renderData._subMeshIdx;

			_meshInstanceData._renderObjects.emplace_back(renderData);

			m_InstanceQueue[_uuid]._meshInstanceDatas.emplace_back(std::move(_meshInstanceData));
		}
	}


	void InstanceRenderer::RegistInstanceQueue(RenderQueue& renderQueue)
	{
		/*while (renderQueue.Empty())
		{
			auto _renderData = renderQueue.Pop();

			if (_renderData._renderObject->GetMaterialBuffer(_renderData._materialIdx)->GetUseInstancing())
			{
				continue;
			}

			RegistInstanceQueue(_renderData);
		}*/
	}

	void InstanceRenderer::ClearInstanceQueue()
	{
		for (auto& _iter : m_InstanceQueue)
		{
			for (auto& _meshInstance : _iter.second._meshInstanceDatas)
			{
				_meshInstance._renderObjects.clear();
			}
		}
	}

	void InstanceRenderer::BindRenderPass(RenderPass* renderPass)
	{
		renderPass->Bind(m_CommandBuffer);
	}

	void InstanceRenderer::BeginExcute()
	{

	}

	void InstanceRenderer::Excute()
	{
		for (auto& _iter : m_InstanceQueue)
		{
			for (auto& _meshInstance : _iter.second._meshInstanceDatas)
			{
				uint32 _meshCount = 0;

				do 
				{
					uint32 _drawNum = UpdateInstanceBuffer(_meshInstance, _meshCount);

					RenderPass* _renderPass = GetRenderPass(_iter.second._materialBuffer, _meshInstance._bIsSkin);

					BindRenderPass(_renderPass);

					_iter.second._materialBuffer->BindResource(m_CommandBuffer);

					auto* _vertexBuffer = _meshInstance._meshBuffer->GetBuffer();

					auto& _subMeshBuffer = _meshInstance._meshBuffer->GetSubMesh(_meshInstance._subMeshIdx);

					Buffer* _instance[2] = { _vertexBuffer , m_InstanceBuffer->GetInstanceBuffer() };

					m_CommandBuffer->SetVertexBuffer(_instance);

					m_CommandBuffer->SetIndexBuffer(*_subMeshBuffer.GetBuffer());

					m_CommandBuffer->DrawIndexedInstanced(_subMeshBuffer.GetIndexCount(), _drawNum, 0);

					_meshCount += _drawNum;
				} 
				while (_meshInstance._renderObjects.size() > _meshCount);
			}
		}
	}

	void InstanceRenderer::EndExcute()
	{

	}

	void InstanceRenderer::Initialize()
	{
		uint32 _size = sizeof(Matrix) * 2 + sizeof(Matrix) * 128 + sizeof(Vector4) * 3;

		m_InstanceBuffer = std::make_shared<InstanceBuffer>(m_RenderSystem, TEXT("Instance Buffer"), _size * SKIN_INSTANCE_NUM);

		m_Deferred_Mesh_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Instance Pass"));

		m_Deferred_Mesh_Albedo_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Instance_Albedo Pass"));

		m_Deferred_Mesh_Albedo_Bump_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Instance_Albedo_Bump Pass"));

		m_Deferred_Mesh_Albedo_Bump_MRA_Pass = m_ResourceManager->GetRenderPass(TEXT("Deferred_Mesh_Instance_Albedo_Bump_MRA Pass"));
	}

	uint32 InstanceRenderer::UpdateInstanceBuffer(MeshInstanceData& meshInstanceData, uint32 startNum)
	{
		uint32 _cnt = 0;

		if (meshInstanceData._bIsSkin)
		{
			for (uint32 i = startNum; i < meshInstanceData._renderObjects.size() || i < startNum + STATIC_INSTANCE_NUM; i++, _cnt++)
			{
				RenderData& _renderData = meshInstanceData._renderObjects[i];

				m_InstanceBuffer->SetInstanceData(&_renderData._renderObject->m_TransformMatrix
					, _renderData._renderObject->m_pSkinnedData
					, _renderData._renderObject->m_MaterialBuffers[_renderData._materialIdx]->GetInstanceData()
					, i);
			}
		}
		else
		{
			for (uint32 i = startNum; i < meshInstanceData._renderObjects.size() || i < startNum + SKIN_INSTANCE_NUM; i++, _cnt++)
			{
				RenderData& _renderData = meshInstanceData._renderObjects[i];

				m_InstanceBuffer->SetInstanceData(&_renderData._renderObject->m_TransformMatrix
					, _renderData._renderObject->m_MaterialBuffers[_renderData._materialIdx]->GetInstanceData()
					, i);
			}
		}

		m_InstanceBuffer->UpdateInstanceBuffer(m_CommandBuffer);

		return _cnt;
	}

	Graphics::RenderPass* InstanceRenderer::GetRenderPass(MaterialBuffer* materialBuffer, bool skin)
	{
		// todo : 퍼티리얼 버퍼가 렌더 패스에 대한 포인터를 가지고 있게 하는게 맞을꺼같은데 일단은 지금은 이렇게 해둠
		if (!skin)
		{
			switch (materialBuffer->GetTextureBindFlags())
			{
				case (uint32)TextureBindFlag::NONE:
				{
					return m_Deferred_Mesh_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO:
				{
					return m_Deferred_Mesh_Albedo_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO_NORMAL:
				{
					return m_Deferred_Mesh_Albedo_Bump_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO_NORMAL_MRA:
				{
					return m_Deferred_Mesh_Albedo_Bump_MRA_Pass.get();
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}
		else
		{
			switch (materialBuffer->GetTextureBindFlags())
			{
				case (uint32)TextureBindFlag::NONE:
				{
					return m_Deferred_Mesh_Skinned_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO:
				{
					return m_Deferred_Mesh_Skinned_Albedo_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO_NORMAL:
				{
					return m_Deferred_Mesh_Skinned_Albedo_Bump_Pass.get();
				}
				case (uint32)TextureBindFlag::ALBEDO_NORMAL_MRA:
				{
					return m_Deferred_Mesh_Skinned_Albedo_Bump_MRA_Pass.get();
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}

		return nullptr;
	}

}