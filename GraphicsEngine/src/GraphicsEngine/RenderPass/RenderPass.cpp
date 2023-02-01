#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/ConstantBuffer.h"

#include "GraphicsModule/Core/RenderTarget.h"
#include "GraphicsModule/Core/CommandBuffer.h"


namespace Graphics
{
	RenderPass::RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget)
		: m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
	{

	}

	RenderPass::RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears)
		: m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
		, m_AttachmentClears(attachmentClears)
	{

	}

	void RenderPass::RegistRenderObject(class RenderObject& renderObject)
	{
		m_RenderObjects.push_back(renderObject);
	}

	void RenderPass::ClearRenderObject()
	{
		m_RenderObjects.clear();
	}

	void RenderPass::UpdatePerFrame(CommandBuffer* commandBuffer, void* src, uint32 size)
	{
		if (m_PerFrameBuffer == nullptr) return;

		commandBuffer->UpdateBuffer(*m_PerFrameBuffer, 0, src, size);
	}

	void RenderPass::BeginExcute(CommandBuffer* commandBuffer, PerFrame* perFrameData)
	{
		commandBuffer->SetPipelineState(*m_PipelineState);
		commandBuffer->SetRenderTarget(*m_RenderTarget, static_cast<uint32>(m_AttachmentClears.size()), m_AttachmentClears.data());

		//commandBuffer->UpdateBuffer(*m_PerFrameBuffer, 0, perFrameData, sizeof(PerFrame));
	}

	void RenderPass::Excute(CommandBuffer* commandBuffer)
	{
		for (size_t _index = 0; _index < m_RenderObjects.size(); _index++)
		{
			auto _vertexBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetBuffer();

			commandBuffer->SetVertexBuffer(*_vertexBuffer);

			UpdateResourcePerObject(commandBuffer, m_RenderObjects[_index]);
			
			if (m_RenderObjects[_index].IsHasViewport())
			{
				commandBuffer->SetViewport(m_RenderObjects[_index].GetViewport());
			}

			for (uint32 _subMeshCnt = 0; _subMeshCnt < m_RenderObjects[_index].GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				UpdateResourcePerMaterial(commandBuffer, m_RenderObjects[_index]);

				commandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				commandBuffer->SetResources(*m_PipelineLayout);

				commandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}
	}

	void RenderPass::EndExcute(CommandBuffer* commandBuffer)
	{
		commandBuffer->EndRenderPass();

		ClearRenderObject();
	}

	void RenderPass::UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject& renderObject)
	{
		auto& _sources = renderObject.GetUpdateResourceData();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			switch (_sources[i]._resourceType)
			{
				case ResourceType::Buffer:
				{
					auto _buffer = m_PipelineLayout->GetBuffer(_sources[i]._index);

					UpdateBuffer(commandBuffer, _buffer, _sources[i]._dataSrc, _sources[i]._datasize);
					break;
				}
				case ResourceType::Sampler:
				case ResourceType::Texture:
				{
					m_PipelineLayout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}
				case ResourceType::Undefined:
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}

	void RenderPass::UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject& renderObject)
	{
		auto& _sources = renderObject.GetUpdateResourceDataPerObject();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			switch (_sources[i]._resourceType)
			{
				case ResourceType::Buffer:
				{
					auto _buffer = m_PipelineLayout->GetBuffer(_sources[i]._index);

					UpdateBuffer(commandBuffer, _buffer, _sources[i]._dataSrc, _sources[i]._datasize);
					break;
				}
				case ResourceType::Sampler:
				case ResourceType::Texture:
				{
					m_PipelineLayout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}
				case ResourceType::Undefined:
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}

	void RenderPass::UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size)
	{
		commandBuffer->UpdateBuffer(*buffer, 0, src, size);
	}

}