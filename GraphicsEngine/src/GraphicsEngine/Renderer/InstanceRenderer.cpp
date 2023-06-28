﻿#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/InstanceBuffer.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"


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

	void InstanceRenderer::RegistInstanceQueue(const RenderData* renderData)
	{
		if (renderData->_materialIdx != -1 && renderData->_renderObject->m_MaterialBuffers.size() < renderData->_materialIdx)
		{
			MaterialBuffer* _matBuffer = renderData->_renderObject->m_MaterialBuffers[renderData->_materialIdx];

			auto& _uuid = _matBuffer->GetUUID();

			for (uint32 i = 0; i < m_InstanceQueue[_uuid]._meshInstanceDatas.size(); i++)
			{
				auto& _materialInstance = m_InstanceQueue[_uuid];

				if (_materialInstance._meshInstanceDatas[i]._meshBuffer->GetUUID()
					== renderData->_renderObject->m_MeshBuffer->GetUUID()
					&& _materialInstance._meshInstanceDatas[i]._subMeshIdx == renderData->_subMeshIdx)
				{
					_materialInstance._meshInstanceDatas[i]._renderObjects.emplace_back(*renderData);

					return;
				}
			}

			MeshInstanceData _meshInstanceData;

			_meshInstanceData._meshBuffer = renderData->_renderObject->m_MeshBuffer;

			_meshInstanceData._bIsSkin = renderData->_renderObject->m_bIsSkinned;

			_meshInstanceData._subMeshIdx = renderData->_subMeshIdx;

			_meshInstanceData._renderObjects.emplace_back(*renderData);

			m_InstanceQueue[_uuid]._meshInstanceDatas.emplace_back(std::move(_meshInstanceData));
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
					_meshCount += UpdateInstanceBuffer(_meshInstance, _meshCount);

					RenderPass* _renderPass = nullptr;

					switch (_iter.second._materialBuffer->GetTextureBindFlags())
					{
						case (uint32)TextureBindFlag::NONE:
						{
							_renderPass = m_Deferred_Mesh_Pass.get();
							break;
						}
						case (uint32)TextureBindFlag::ALBEDO:
						{
							_renderPass = m_Deferred_Mesh_Albedo_Pass.get();
							break;
						}
						case (uint32)TextureBindFlag::ALBEDO_NORMAL:
						{
							_renderPass = m_Deferred_Mesh_Albedo_Bump_Pass.get();
							break;
						}
						case (uint32)TextureBindFlag::ALBEDO_NORMAL_MRA:
						{
							_renderPass = m_Deferred_Mesh_Albedo_Bump_MRA_Pass.get();
							break;
						}
						default:
							assert(false);
							break;
					}

					BindRenderPass(_renderPass);

					_iter.second._materialBuffer->BindResource(m_CommandBuffer);

					auto* _vertexBuffer = _meshInstance._meshBuffer->GetBuffer();

					auto& _subMeshBuffer = _meshInstance._meshBuffer->GetSubMesh(_meshInstance._subMeshIdx);

					m_CommandBuffer->SetVertexBuffer(*_vertexBuffer);

					m_CommandBuffer->SetIndexBuffer(*_subMeshBuffer.GetBuffer());

					m_CommandBuffer->DrawIndexedInstanced(_subMeshBuffer.GetIndexCount(), _meshInstance._renderObjects.size(), 0);
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

}