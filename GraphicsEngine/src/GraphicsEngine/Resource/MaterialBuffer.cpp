#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

namespace Graphics
{
	
	MaterialBuffer::MaterialBuffer(Graphics::RenderSystem* renderSystem, Graphics::PipelineLayout* pipelineLayout)
		: ResourceBuffer(renderSystem, BufferType::Material)
		, m_PipelineLayout(pipelineLayout)
	{

	}

	MaterialBuffer::~MaterialBuffer()
	{
		m_RenderSystem->Release(*m_PipelineLayout);
		m_PipelineLayout = nullptr;
	}

	void MaterialBuffer::SetResource(uint32 i, Graphics::Resource* resource)
	{
		m_PipelineLayout->SetResource(i, resource);
	}

}