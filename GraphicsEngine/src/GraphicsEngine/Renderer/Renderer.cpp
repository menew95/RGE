#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Renderer/Renderer.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

namespace Graphics
{
	Renderer::Renderer(RenderSystem* renderSystem, ResourceManager* resourceManager)
		: m_RenderSystem(renderSystem)
		, m_ResourceManager(resourceManager)
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::BindRenderPass(CommandBuffer* commandBuffer, RenderPass* renderPass)
	{
		if (renderPass == nullptr) assert(false);

		renderPass->Bind(commandBuffer);
	}

	void Renderer::Excute(CommandBuffer* commandBuffer, RenderPass* renderPass, std::vector<RenderData>& renderDatas)
	{
		//BindRenderPass(commandBuffer, renderPass);

		renderPass->BeginExcute(commandBuffer);

		renderPass->Excute(commandBuffer, renderDatas);

		renderPass->EndExcute(commandBuffer);
	}
}