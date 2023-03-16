#pragma once

#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* PostProcess 관련 렌더패스를 다루는 클래스 일단은 SSR 하나
*/

namespace Graphics
{
	class RenderPass;
	class CommandBuffer;
	class ResourceManager;

	class PostProcess
	{
	public:
		PostProcess(CommandBuffer* command, ResourceManager* resourceManager);
		~PostProcess();

		void ExcutePass();

		void SetRenderTarget(RenderTarget* renderTarget);

	private:
		void Initialize();

		ResourceManager* m_ResourceManager;
		CommandBuffer* m_CommandBuffer;

		RenderObject m_RenderObjects;

		std::shared_ptr<Graphics::RenderPass> m_PostProcess_Pass;
	};
}