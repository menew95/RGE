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
				// m_Bindings에 기록된 바인딩 정보와 다른 리소스를 저장하려고 함
				assert(false);
				return;
			}

			// Todo : 슬록, 바인딩 플래그, 스테이지 플래그도 비교가 필요할수도?
			m_Resources[index] = resource;
		}

		uint32 DX11PipelineLayout::GetNumBindings() const
		{
			return static_cast<uint32>(m_Bindings.size());
		}
	}
}