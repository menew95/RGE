#include "Common.h"

#include "GraphicsEngine/RenderObject.h"

/*
* 디퍼드 버퍼들을 출력
*/

namespace Graphics
{
	class CameraBuffer;
	class RenderPass;
	class CommandBuffer;
	class ResourceManager;

	class DebugDeferred
	{
	public:
		DebugDeferred(CommandBuffer* command, ResourceManager* resourceManager);
		~DebugDeferred();

		void ExcutePass();

		void SetSwapChain(class SwapChain* swapchain);

	private:
		void Initialize();

		ResourceManager* m_ResourceManager;
		CommandBuffer* m_CommandBuffer;

		std::shared_ptr<Graphics::RenderPass> m_Debug_Pass;

		// Deferrd resource for debug
		std::vector<RenderObject> m_DebugRenderObject;
	};
}