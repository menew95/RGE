#include "GraphicsEnginePCH.h"
#include "GraphicsEngine/Resource/MaterialBuffer.h"

#include "GraphicsEngine/RenderObject.h"
#include "GraphicsEngine/RenderPass/RenderPass.h"


namespace Graphics
{
	
	MaterialBuffer::MaterialBuffer(Graphics::RenderSystem* renderSystem, uuid uuid)
		: ResourceBuffer(renderSystem, BufferType::Material, uuid)
		, m_TextureBindFlag((uint32)TextureBindFlag::NONE)
		, m_RenderPass(nullptr)
	{

	}

	MaterialBuffer::~MaterialBuffer()
	{

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

		// Todo : 일단 여기서 사용하자 아직 여러개의 머티리얼을 가정할 단계가 아니니 PBR 머티리얼 일경우에만으로 치자
		memcpy(&m_PBRMaterialData, src, 12);
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

	void MaterialBuffer::ChangeResource(Resource* resource, uint32 idx)
	{
		assert(m_ResourceBindList.size() > idx);

		assert(m_ResourceBindList[idx]._resource->GetResourceType() == resource->GetResourceType());

		m_ResourceBindList[idx]._resource = resource;
	}

}