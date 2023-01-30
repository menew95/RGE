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
	RenderPass::RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget)
		: m_PipelineState(pipelineState)
		, m_RenderTarget(renderTarget)
	{

	}

	RenderPass::RenderPass(PipelineState* pipelineState, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears)
		: m_PipelineState(pipelineState)
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

	void RenderPass::UpdatePerDraw(CommandBuffer* commandBuffer, void* src, uint32 size)
	{
		if (m_PerDrawBuffer == nullptr) return;

		commandBuffer->UpdateBuffer(*m_PerDrawBuffer, 0, src, size);
	}

	void RenderPass::BeginExcute(CommandBuffer* commandBuffer, PerFrame* perFrameData)
	{
		commandBuffer->SetPipelineState(*m_PipelineState);
		commandBuffer->SetRenderTarget(*m_RenderTarget, m_AttachmentClears.size(), m_AttachmentClears.data());


		Math::Viewport _viewport{0, 0, 1280, 720, 0, 1};

		//commandBuffer->SetViewport(_viewport);
		//commandBuffer->UpdateBuffer(*m_PerFrameBuffer, 0, perFrameData, sizeof(PerFrame));
	}

	void RenderPass::Excute(CommandBuffer* commandBuffer)
	{
		for (size_t _index = 0; _index < m_RenderObjects.size(); _index++)
		{
			auto _vertexBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetBuffer();

			commandBuffer->SetVertexBuffer(*_vertexBuffer);

			UpdatePerDraw(commandBuffer, m_RenderObjects[_index].m_UpdateResourcePerDraw._dataSrc, m_RenderObjects[_index].m_UpdateResourcePerDraw._datasize);

			for (uint32 _subMeshCnt = 0; _subMeshCnt < m_RenderObjects[_index].GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = m_RenderObjects[_index].GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				auto _pipelineLayout = m_RenderObjects[_index].GetMaterialBuffer()->GetPipelineLayout();

				UpdateResource(commandBuffer, m_RenderObjects[_index]);

				commandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				commandBuffer->SetResources(*_pipelineLayout);

				commandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}
	}

	void RenderPass::EndExcute(CommandBuffer* commandBuffer)
	{
		commandBuffer->EndRenderPass();

		ClearRenderObject();
	}

	void RenderPass::UpdateConstBuffer(CommandBuffer* commandBuffer, RenderObject& renderObject)
	{
		// 재설계중
		assert(false);

		auto& _buffers = renderObject.GetConstBuffers();
		auto& _sources = renderObject.GetUpdateResourceData();


		for (size_t i = 0; i < _sources.size(); i++)
		{
			commandBuffer->UpdateBuffer(*m_PerDrawBuffer, 0, _sources[0]._dataSrc, _sources[0]._datasize);
		}
	}

	void RenderPass::UpdateResource(CommandBuffer* commandBuffer, RenderObject& renderObject)
	{
		auto* _layout = renderObject.GetMaterialBuffer()->GetPipelineLayout();
		auto& _sources = renderObject.GetUpdateResourceData();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			assert(_sources[i]._resourceType != ResourceType::Undefined);

			switch (_sources[i]._resourceType)
			{
				case ResourceType::Buffer:
				{
					break;
				}
				case ResourceType::Sampler:
				case ResourceType::Texture:
				{
					_layout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}
				/*{
					_layout->SetResource(_sources[i]._index, reinterpret_cast<Resource*>(_sources[i]._dataSrc));
					break;
				}*/
				case ResourceType::Undefined:
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}

}