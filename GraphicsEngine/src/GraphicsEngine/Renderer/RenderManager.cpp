#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/RenderManager.h"
#include "GraphicsEngine/Renderer/Renderer.h"
#include "GraphicsEngine/Renderer/InstanceRenderer.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"
#include "GraphicsEngine/Resource/MeshBuffer.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

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

	void RenderManager::Begin()
	{
		RenderQueue _queue;

		RenderQueueManager::Get()->CreateRenderQueue(_queue);

		//while (_queue.Empty())
		//{
		//	RenderData _renderData;// = _queue.Pop();

		//	MaterialBuffer* _matBuf = _renderData._renderObject->GetMaterialBuffer(_renderData._materialIdx);

		//	if (_matBuf->GetUseInstancing())
		//	{
		//		m_InstanceRenderer->RegistInstanceQueue(_renderData);
		//	}
		//	else
		//	{
		//		_matBuf->GetPass()->AddRenderData(_renderData);
		//	}
		//}
	}

	void RenderManager::Excute(CommandBuffer* commandBuffer)
	{
		{
			m_InstanceRenderer->BeginExcute();

			m_InstanceRenderer->Excute();

			m_InstanceRenderer->EndExcute();
		}
	}

	void RenderManager::End()
	{

	}

	void RenderManager::InitRenderer()
	{
		m_DefualtRenderer = std::make_shared<Renderer>(m_RenderSystem, m_ResourceManager);

		m_InstanceRenderer = std::make_shared<InstanceRenderer>(m_RenderSystem, m_ResourceManager, m_CommandBuffer);
	}

}