#pragma once

#include "GraphicsModule/Core/PipelineLayout.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11PipelineLayout : public PipelineLayout
		{
		public:
			DX11PipelineLayout(const PipelineLayoutDesc& desc);
			virtual ~DX11PipelineLayout();

			inline const std::vector<BindingDescriptor>& GetBindings() const
			{
				return m_Bindings;
			}

			uint32 GetNumBindings() const override;

		private:

			std::vector<BindingDescriptor> m_Bindings;

		};
	}
}