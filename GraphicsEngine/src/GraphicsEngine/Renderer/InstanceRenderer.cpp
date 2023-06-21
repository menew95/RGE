#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
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

	void InstanceRenderer::UpdateInstanceBuffer(MeshInstanceData& meshInstanceData)
	{
		if (meshInstanceData._bIsSkin)
		{
			for (uint32 i = 0; i < meshInstanceData._renderObjects.size(); i++)
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
			for (uint32 i = 0; i < meshInstanceData._renderObjects.size(); i++)
			{
				RenderData& _renderData = meshInstanceData._renderObjects[i];

				m_InstanceBuffer->SetInstanceData(&_renderData._renderObject->m_TransformMatrix
					, _renderData._renderObject->m_MaterialBuffers[_renderData._materialIdx]->GetInstanceData()
					, i);
			}
		}

		m_InstanceBuffer->UpdateInstanceBuffer(m_CommandBuffer);
	}

	void InstanceRenderer::Initialize()
	{
		uint32 _size = sizeof(Matrix) * 2 + sizeof(Matrix) * 128 + sizeof(Vector4) * 3;

		m_InstanceBuffer = std::make_shared<InstanceBuffer>(m_RenderSystem, TEXT("Instance Buffer"), _size * 7 * 16);
	}

}