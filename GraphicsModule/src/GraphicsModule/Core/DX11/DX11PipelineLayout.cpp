#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11PipelineLayout.h"

namespace Graphics
{
	namespace DX11
	{
		DX11PipelineLayout::DX11PipelineLayout(const PipelineLayoutDesc& desc)
		{
			m_Bindings = desc.bindings;
		}

		DX11PipelineLayout::~DX11PipelineLayout()
		{

		}

		uint32 DX11PipelineLayout::GetNumBindings() const
		{
			return static_cast<uint32>(m_Bindings.size());
		}
	}
}