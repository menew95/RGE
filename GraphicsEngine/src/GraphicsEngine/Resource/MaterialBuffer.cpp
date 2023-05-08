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

	void MaterialBuffer::SetResource(Resource* resource, ResourceType type, uint32 bindSlot, uint32 size)
	{
		Graphics::UpdateResourceData _resourceData;
		_resourceData._datasize = size;
		_resourceData._dataSrc = resource;
		_resourceData._resourceType = type;
		_resourceData._updateTime = eUpdateTime::PerMaterial;
		
		m_UpdateResources.push_back(_resourceData);


		ResourceBind _resource;
		_resource._resource = resource;
		_resource._slot = bindSlot;

		m_ResourceBindList.push_back(_resource);
	}

	void MaterialBuffer::BindResource(CommandBuffer* commandBuffer)
	{
		for (size_t i = 0; i < m_UpdateResources.size(); i++)
		{
			switch (m_UpdateResources[i]._resourceType)
			{
				case ResourceType::Buffer:
					assert(false);
					//m_ResourceList[m_UpdateResources[i]._index].
					//Buffer* _buffer = reinterpret_cast<Buffer*>(m_ResourceList[m_UpdateResources[i]._index]);
					
					//commandBuffer->UpdateBuffer(*_buffer, 0, m_UpdateResources[i]._dataSrc, m_UpdateResources[i]._datasize);
					break;
				case ResourceType::Sampler:
				case ResourceType::Texture:
					commandBuffer->SetResource(*(m_ResourceBindList[i]._resource), m_ResourceBindList[i]._slot, BindFlags::ShaderResource, StageFlags::PS);
					break;
				case ResourceType::Undefined:
				default:
					assert(false);
					break;
			}
			
		}
	}

}