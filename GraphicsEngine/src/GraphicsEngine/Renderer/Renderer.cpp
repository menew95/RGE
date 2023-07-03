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

	void Renderer::BeginExcute(CommandBuffer* commandBuffer)
	{

	}

	void Renderer::Excute(CommandBuffer* commandBuffer, RenderPass* renderPass, std::vector<RenderData>& renderDatas)
	{
		renderPass->BeginExcute(commandBuffer);

		renderPass->Excute(commandBuffer, renderDatas);

		renderPass->EndExcute(commandBuffer);
	}

	void Renderer::EndExcute(CommandBuffer* commandBuffer)
	{

	}

}