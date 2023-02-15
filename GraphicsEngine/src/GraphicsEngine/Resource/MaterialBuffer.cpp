#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"

namespace Graphics
{
	
	MaterialBuffer::MaterialBuffer(Graphics::RenderSystem* renderSystem)
		: ResourceBuffer(renderSystem, BufferType::Material)
	{

	}

	MaterialBuffer::~MaterialBuffer()
	{
		//m_RenderSystem->Release(*m_PipelineLayout);
	}
}