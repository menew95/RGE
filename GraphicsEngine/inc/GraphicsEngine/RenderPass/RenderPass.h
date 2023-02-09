#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderPassFlags.h"

#include "GraphicsEngine/RenderObject.h"

#include "GraphicsEngine/Export.h"

#include "GraphicsModule/Core/CommandBufferFlags.h"


namespace Graphics
{
	class PipelineState;
	class PipelineLayout;
	class RenderTarget;
	class Resource;
	class CommandBuffer;

	struct PerFrame;

	struct ResourceClear
	{
		ResourceType _type;
		uint32 _fristSlot;
		uint32 _numSlots;
		long _bindFlags;
		long _stageFlags;
	};

	class GRAPHICSENGINE_DLL_DECLSPEC RenderPass
	{
	public:
		RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget);
		RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget);
		RenderPass(const tstring& passName, PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears);
		RenderPass(PipelineState* pipelineState, PipelineLayout* pipelineLayout, RenderTarget* renderTarget, std::vector<AttachmentClear> attachmentClears);
		virtual ~RenderPass();

		void RegistRenderObject(class RenderObject& renderObject);
		void ClearRenderObject();

		inline const tstring& GetPassName() const 
		{
			return m_PassName;
		}

		inline const PipelineState* GetPipelineState() const
		{
			return m_PipelineState;
		}

		inline const PipelineLayout* GetPipelineLayout() const
		{
			return m_PipelineLayout;
		}

		inline const RenderTarget* GetRenderTarget() const
		{
			return m_RenderTarget;
		}

		void UpdatePerFrame(CommandBuffer* commandBuffer, void* src, uint32 size);

		void BeginExcute(CommandBuffer* commandBuffer, PerFrame* perFrameData);
		void Excute(CommandBuffer* commandBuffer);
		void EndExcute(CommandBuffer* commandBuffer);

		inline void SetPassName(const tstring& name) { m_PassName = name; }

		inline void SetPerFrameBuffer(Buffer* perFrame) { m_PerFrameBuffer = perFrame; }

		inline void SetRenderTarget(RenderTarget* renderTarget) { m_RenderTarget = renderTarget; }

		inline void SetClearObjects(bool value) { m_IsClearObjects = value; }

		inline void SetResourceClear(std::vector<ResourceClear>& clears) { m_ResourceClears = clears; }
		inline void AddResourceClear(ResourceClear clear) { m_ResourceClears.push_back(clear); }

	protected:
		void UpdateResourcePerMaterial(CommandBuffer* commandBuffer, RenderObject& renderObject);
		void UpdateResourcePerObject(CommandBuffer* commandBuffer, RenderObject& renderObject);

		void UpdateBuffer(CommandBuffer* commandBuffer, Buffer* buffer, void* src, uint32 size);

		tstring m_PassName;

		PipelineState* m_PipelineState = nullptr;
		PipelineLayout* m_PipelineLayout = nullptr;
		RenderTarget* m_RenderTarget = nullptr;

		std::vector<Resource*> m_Resources;

		std::vector<RenderObject> m_RenderObjects;

		std::vector<AttachmentClear> m_AttachmentClears;

		Buffer* m_PerFrameBuffer;

		bool m_IsClearObjects = true;

		std::vector<ResourceClear> m_ResourceClears;
	};
}
