#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/RenderManager.h"
#include "GraphicsEngine/Renderer/Renderer.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/RenderQueue/RenderQueueManager.h"
#include "GraphicsEngine/RenderQueue/RenderQueue.h"

namespace Graphics
{
	RenderManager::RenderManager(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
		, m_CommandBuffer(commandBuffer)
		, m_DefualtRenderer(nullptr)
	{
		InitRenderer();
	}

	RenderManager::~RenderManager()
	{

	}

	void RenderManager::Excute(CommandBuffer* commandBuffer)
	{
		RenderQueue _queue;

		RenderQueueManager::Get()->CreateRenderQueue(_queue);

		m_InstanceRenderer->RegistInstanceQueue(_queue);
	}

	void RenderManager::InitRenderer()
	{
		m_DefualtRenderer = std::make_shared<Renderer>(m_RenderSystem, m_ResourceManager);

		m_InstanceRenderer = std::make_shared<InstanceRenderer>(m_RenderSystem, m_ResourceManager, m_CommandBuffer);
	}

}