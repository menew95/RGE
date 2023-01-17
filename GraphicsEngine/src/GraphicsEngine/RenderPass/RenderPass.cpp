#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/CommandBuffer.h"

namespace Graphics
{
	RenderPass::RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget)
		: m_PipelineState(pipelineState)
		, m_RenderTarget(renderTarget)
	{

	}

	void RenderPass::RegistRenderObject(class RenderObject& renderObject)
	{
		m_RenderObjects.push_back(renderObject);
	}

	void RenderPass::BeginExcute(CommandBuffer* commandBuffer)
	{
		AttachmentClear _attachmentClear[] =
		{
			{ {1,0,0,0}, 0 },
			{ {0,0,0,0}, 1},
			{ {0,0,0,0}, 2},
			{ {0,0,0,0}, 3},
			{ {0,0,0,0}, 4},
			{ 1, 0 }
		};

		commandBuffer->SetPipelineState(*m_PipelineState);
		commandBuffer->SetRenderTarget(*m_RenderTarget, 6, _attachmentClear);
	}

	void RenderPass::Excute(CommandBuffer* commandBuffer)
	{
		for (size_t _index = 0; _index < m_RenderObjects.size(); _index++)
		{
			auto _vertexBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetBuffer();

			commandBuffer->SetVertexBuffer(_vertexBuffer);

			for (uint32 _subMeshCnt = 0; _subMeshCnt < m_RenderObjects[_index].GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				auto _pipelineLayout = m_RenderObjects[_index].GetMaterialBuffer()->GetPipelineLayout();

				UpdateConstBuffer(commandBuffer, m_RenderObjects[_index]);

				commandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				commandBuffer->SetResources(*_pipelineLayout);

				commandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}
	}

	void RenderPass::EndExcute(CommandBuffer* commandBuffer)
	{
		commandBuffer->EndRenderPass();
	}

	void RenderPass::UpdateConstBuffer(CommandBuffer* commandBuffer, RenderObject& renderObject)
	{
		auto& _buffers = renderObject.GetConstBuffers();
		auto& _sources = renderObject.GetUpdateResourceData();

		for (size_t i = 0; i < _buffers.size(); i++)
		{
			commandBuffer->UpdateBuffer(*(_buffers[i]), 0, _sources[i]._dataSrc, _sources[i]._datasize);
		}
	}

}