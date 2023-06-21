#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/RenderManager.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"

namespace Graphics
{
	RenderManager::RenderManager(RenderSystem* renderSystem, ResourceManager* resourceManager)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
	{
		InitRenderer();
	}

	RenderManager::~RenderManager()
	{

	}

	void RenderManager::InitRenderer()
	{
		m_InstanceRenderer = std::make_shared<InstanceRenderer>(m_RenderSystem, m_ResourceManager);
	}

}