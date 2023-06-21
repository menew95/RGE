#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/Resource/ResourceManager.h"
#include "GraphicsEngine/Resource/InstanceBuffer.h"

namespace Graphics
{
	InstanceRenderer::InstanceRenderer(RenderSystem* renderSystem, ResourceManager* resourceManager)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
	{
		Initialize();
	}

	InstanceRenderer::~InstanceRenderer()
	{

	}

	void InstanceRenderer::RegistInstanceQueue(const RenderData* renderData)
	{

	}

	void InstanceRenderer::UpdateInstanceBuffer()
	{

	}

	void InstanceRenderer::Initialize()
	{
		uint32 _size = sizeof(Matrix) * 2 + sizeof(Matrix) * 128 + sizeof(Vector4) * 3;

		m_InstanceBuffer = std::make_shared<InstanceBuffer>(m_RenderSystem, TEXT("Instance Buffer"), _size * 7 * 16);
	}

}