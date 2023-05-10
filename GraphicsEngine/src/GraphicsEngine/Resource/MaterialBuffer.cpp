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

	void MaterialBuffer::SetBufferData(void* src, Resource* resource)
	{
		ResourceBind _resource;
		_resource._resource = resource;
		m_ResourceBindList.emplace_back(_resource);

		Graphics::UpdateResourceData _resourceData;
		_resourceData._datasize = sizeof(Standard);
		_resourceData._dataSrc = src;
		_resourceData._resourceType = ResourceType::Buffer;
		_resourceData._updateTime = eUpdateTime::PerMaterial;

		_resourceData._index = static_cast<uint32>(m_ResourceBindList.size()) - 1u;
		m_UpdateResources.emplace_back(_resourceData);
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
				{
					Buffer* _buffer = reinterpret_cast<Buffer*>(m_ResourceBindList[m_UpdateResources[i]._index]._resource);
					
					commandBuffer->UpdateBuffer(*_buffer, 0, m_UpdateResources[i]._dataSrc, m_UpdateResources[i]._datasize);
					break;
				}
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