#include "GraphicsPCH.h"
#include "GraphicsModule/Core/Resource.h"
#include "GraphicsModule/Core/DX11/DX11PipelineLayout.h"

namespace Graphics
{
	namespace DX11
	{
		DX11PipelineLayout::DX11PipelineLayout(const PipelineLayoutDesc& desc)
			: m_Bindings(desc._bindings)
			, m_Resources(desc._resources)
		{
			m_Bindings = desc._bindings;
		}

		DX11PipelineLayout::~DX11PipelineLayout()
		{

		}

		void DX11PipelineLayout::SetResources(std::vector<Resource*>& resources)
		{
			for (uint32 i = 0; i < GetNumBindings(); i++)
			{
				SetResource(i, resources[i]);
			}
		}

		void DX11PipelineLayout::SetResource(uint32 index, Resource* resource)
		{
			if (index >= m_Bindings.size())
				return;

			if (resource->GetResourceType() != m_Bindings[index]._type)
			{
				// m_Bindings�� ��ϵ� ���ε� ������ �ٸ� ���ҽ��� �����Ϸ��� ��
				assert(false);
				return;
			}

			// Todo : ����, ���ε� �÷���, �������� �÷��׵� �񱳰� �ʿ��Ҽ���?
			m_Resources[index] = resource;
		}

		uint32 DX11PipelineLayout::GetNumBindings() const
		{
			return static_cast<uint32>(m_Bindings.size());
		}

		Resource* DX11PipelineLayout::GetResource(uint32 index)
		{
			return m_Resources.at(index);
		}

		Graphics::Buffer* DX11PipelineLayout::GetBuffer(uint32 index)
		{
			auto _resource = m_Resources.at(index);

			assert(_resource->GetResourceType() == ResourceType::Buffer);

			return reinterpret_cast<Buffer*>(_resource);
		}

		Graphics::Texture* DX11PipelineLayout::GetTexture(uint32 index)
		{
			auto _resource = m_Resources.at(index);

			assert(_resource->GetResourceType() == ResourceType::Texture);

			return reinterpret_cast<Texture*>(_resource);
		}

		Graphics::Sampler* DX11PipelineLayout::GetSampler(uint32 index)
		{
			auto _resource = m_Resources.at(index);

			assert(_resource->GetResourceType() == ResourceType::Sampler);

			return reinterpret_cast<Sampler*>(_resource);
		}

	}
}