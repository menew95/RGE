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
	RenderPass::RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget)
		: m_PassName(passName)
		, m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
		, m_PerFrameBuffer(nullptr)
	{

	}

	RenderPass::RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget)
		: m_PassName(TEXT("RenderPass"))
		, m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
		, m_PerFrameBuffer(nullptr)
	{

	}

	RenderPass::RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears)
		: m_PassName(passName)
		, m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
		, m_AttachmentClears(attachmentClears)
		, m_PerFrameBuffer(nullptr)
	{

	}

	RenderPass::RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears)
		: m_PassName(TEXT("RenderPass"))
		, m_PipelineState(pipelineState)
		, m_PipelineLayout(pipelineLayout)
		, m_RenderTarget(renderTarget)
		, m_AttachmentClears(attachmentClears)
		, m_PerFrameBuffer(nullptr)
	{

	}

	RenderPass::RenderPass(RenderPassDesc desc)
		: m_PassName(desc._passName)
		, m_PipelineState(desc._pipelineState)
		, m_PipelineLayout(desc._pipelineLayout)
		, m_RenderTarget(desc._renderTarget)
		, m_PerFrameBuffer(desc._perFrameBuffer)
		, m_AttachmentClears(desc._attachmentClears)
		, m_IsClearObjects(desc._IsClearObjects)
		, m_ResourceClears(desc._resourceClears)
	{

	}

	void RenderPass::RegistRenderObject(class RenderObject* renderObject)
	{
		m_RenderObjects.emplace_back(renderObject);
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
#if defined(_DEBUG) || defined(DEBUG)
		commandBuffer->BeginEvent(m_PassName.c_str());
#endif

		commandBuffer->SetPipelineState(*m_PipelineState);
		
		if(m_RenderTarget)
			commandBuffer->SetRenderTarget(*m_RenderTarget, static_cast<uint32>(m_AttachmentClears.size()), m_AttachmentClears.data());

		if (m_PipelineLayout != nullptr)
			commandBuffer->SetResources(*m_PipelineLayout);
	}

	void RenderPass::Excute(CommandBuffer* commandBuffer)
	{
		for (size_t _index = 0; _index < m_RenderObjects.size(); _index++)
		{
			auto _vertexBuffer = m_RenderObjects[_index]->GetMeshBuffer()->GetBuffer();

			commandBuffer->SetVertexBuffer(*_vertexBuffer);

			UpdateResourcePerObject(commandBuffer, m_RenderObjects[_index]);
			
			if (m_RenderObjects[_index]->IsHasViewport())
			{
				commandBuffer->SetViewports(static_cast<uint32>(m_RenderObjects[_index]->GetViewports().size()), m_RenderObjects[_index]->GetViewports().data());
			}

			for (uint32 _subMeshCnt = 0; _subMeshCnt < m_RenderObjects[_index]->GetMeshBuffer()->GetSubMeshCount(); _subMeshCnt++)
			{
				auto _subMeshBuffer = m_RenderObjects[_index]->GetMeshBuffer()->GetSubMesh(_subMeshCnt);

				if (_subMeshCnt < m_RenderObjects[_index]->GetMaterialBuffersCount()
					&& m_RenderObjects[_index]->m_MaterialBuffers[_subMeshCnt] != nullptr)
				{
					m_RenderObjects[_index]->m_MaterialBuffers[_subMeshCnt]->BindResource(commandBuffer);

					//UpdateResourcePerMaterial(commandBuffer, m_RenderObjects[_index]);
				}

				commandBuffer->SetIndexBuffer(*_subMeshBuffer.m_IndexBuffer);

				commandBuffer->DrawIndexed(_subMeshBuffer.m_IndexCount, 0, 0);
			}
		}
	}

	void RenderPass::ExcuteMaterialInstance(CommandBuffer* commnadBuffer, std::vector<MaterialInstanceData>& materialInstanceData)
	{
		for (size_t _instanceIdx = 0; _instanceIdx < materialInstanceData.size(); _instanceIdx++)
		{
			auto& _materialInstance = materialInstanceData[_instanceIdx];

			_materialInstance._materialBuffer->BindResource(commnadBuffer);

			for (size_t _dataIdx = 0; _dataIdx < _materialInstance._meshInstanceDatas.size(); _dataIdx)
			{



			}
		}
	}

	void RenderPass::EndExcute(CommandBuffer* commandBuffer)
	{
		commandBuffer->EndRenderPass();

		if (m_IsClearObjects)
			ClearRenderObject();

		for (auto& _clear : m_ResourceClears)
		{
			commandBuffer->ResetResourceSlots(_clear._type, _clear._fristSlot, _clear._numSlots, _clear._bindFlags, _clear._stageFlags);
		}

#if defined(_DEBUG) || defined(DEBUG)
		commandBuffer->EndEvent();
#endif
	}

	//void RenderPass::UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject* renderObject)
	//{
	//	//auto& _sources = renderObject->m_MaterialBuffer->GetUpdateResourceData();
	//	//
	//	//renderObject->m_MaterialBuffer->BindResource(commandBuffer);

	//	//for (size_t i = 0; i < _sources.size(); i++)
	//	//{
	//	//	assert(m_PipelineLayout != nullptr);

	//	//	switch (_sources[i]._resourceType)
	//	//	{
	//	//		case ResourceType::Buffer:
	//	//		{
	//	//			auto _buffer = m_PipelineLayout->GetBuffer(_sources[i]._index);

	//	//			UpdateBuffer(commandBuffer, _buffer, _sources[i]._dataSrc, _sources[i]._datasize);
	//	//			break;
	//	//		}
	//	//		case ResourceType::Sampler:
	//	//		case ResourceType::Texture:
	//	//		{
	//	//			//commandBuffer->SetResource(reinterpret_cast<Resource*>(_sources[i]._dataSrc), BindFlags::)
	//	//			//m_PipelineLayout->SetResource(_sources[i]._index, );
	//	//			break;
	//	//		}
	//	//		case ResourceType::Undefined:
	//	//		default:
	//	//		{
	//	//			assert(false);
	//	//			break;
	//	//		}
	//	//	}
	//	//}
	//}

	void RenderPass::UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject)
	{
		auto& _sources = renderObject->GetUpdateResourceDataPerObject();

		for (size_t i = 0; i < _sources.size(); i++)
		{
			assert(m_PipelineLayout != nullptr);

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