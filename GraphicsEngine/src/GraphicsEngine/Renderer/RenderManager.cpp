#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/RenderManager.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"

namespace Graphics
{
	RenderManager::RenderManager(RenderSystem* renderSystem, ResourceManager* resourceManager, CommandBuffer* commandBuffer)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
		, m_CommandBuffer(commandBuffer)
	{
		InitRenderer();
	}

	RenderManager::~RenderManager()
	{

	}

	void RenderManager::Excute(CommandBuffer* commandBuffer)
	{

	}

	void RenderManager::InitRenderer()
	{
		m_InstanceRenderer = std::make_shared<InstanceRenderer>(m_RenderSystem, m_ResourceManager, m_CommandBuffer);
	}

}