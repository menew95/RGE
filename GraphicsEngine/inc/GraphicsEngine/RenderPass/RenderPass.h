#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderPass/RenderPassFlags.h"

#include "GraphicsEngine/RenderObject.h"

#include "GraphicsEngine/RenderQueue/RenderQueue.h"

#include "GraphicsEngine/Export.h"

namespace Graphics
{
	class PipelineState;
	class PipelineLayout;
	class RenderTarget;
	class Resource;
	class CommandBuffer;
	class RenderQueue;
	struct PerFrame;
	struct RenderData;

	class GRAPHICSENGINE_DLL_DECLSPEC RenderPass
	{
	public:
		RenderPass(RenderPassDesc desc);
		
		RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget);
		
		RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget);
		
		RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears);
		
		RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears);
		
		virtual ~RenderPass();

		void RegistRenderObject(class RenderObject* renderObject);

		void ClearRenderObject();

		inline const tstring& GetPassName() const 
		{
			return m_PassName;
		}

		inline const PipelineState& GetPipelineState() const
		{
			return *m_PipelineState;
		}

		inline const PipelineLayout& GetPipelineLayout() const
		{
			return *m_PipelineLayout;
		}

		inline const RenderTarget& GetRenderTarget() const
		{
			return *m_RenderTarget;
		}

		void UpdatePerFrame(CommandBuffer* commandBuffer, void* src, uint32 size);

		void BeginExcute(CommandBuffer* commandBuffer, PerFrame* perFrameData = nullptr);

		void Excute(CommandBuffer* commandBuffer);

		void Excute(CommandBuffer* commandBuffer, std::vector<RenderData>& renderDatas);

		void EndExcute(CommandBuffer* commandBuffer);

		/**
			@brief 설정된 렌더링 파이프 라인을 바인드 
			@param commandBuffer - 컨택스트
		**/
		void Bind(CommandBuffer* commandBuffer);

		void Excute(CommandBuffer* commandBuffer, const RenderQueue& renderQueue);

		inline void SetPassName(const tstring& name) { m_PassName = name; }

		inline void SetPerFrameBuffer(Buffer* perFrame) { m_PerFrameBuffer = perFrame; }

		inline void SetRenderTarget(RenderTarget* renderTarget) { m_RenderTarget = renderTarget; }

		inline void SetClearObjects(bool value) { m_IsClearObjects = value; }

		inline void AddResourceClear(ResourceClear clear) { m_ResourceClears.push_back(clear); }

		inline void SetResourceClear(std::vector<ResourceClear>& clears) { m_ResourceClears = clears; }

		inline std::vector<ResourceClear> GetResourceClear() { return m_ResourceClears; }

		inline void SetAttachmentClears(std::vector<AttachmentClear> clears) { m_AttachmentClears = clears; }

		inline std::vector<AttachmentClear> GetAttachmentClears() { return m_AttachmentClears; }

		inline void AddRenderData(const RenderData& data) { m_RenderDatas.emplace_back(data); }

		inline auto& GetRenderDatas() { return m_RenderDatas; }

		inline void ClearRenderDatas() { m_RenderDatas.clear(); }

	protected:
		void UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject* renderObject);

		void UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size);

		tstring m_PassName;

		PipelineState* m_PipelineState = nullptr;

		PipelineLayout* m_PipelineLayout = nullptr;

		RenderTarget* m_RenderTarget = nullptr;

		std::vector<Resource*> m_Resources;

		std::vector<RenderObject*> m_RenderObjects;

		std::vector<RenderData> m_RenderDatas;

		std::vector<AttachmentClear> m_AttachmentClears;

		Buffer* m_PerFrameBuffer;

		bool m_IsClearObjects = true;

		std::vector<ResourceClear> m_ResourceClears;

	};
}
