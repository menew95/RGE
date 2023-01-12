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
	}
}