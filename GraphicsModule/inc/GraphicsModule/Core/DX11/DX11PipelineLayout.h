#pragma once

#include "GraphicsModule/Core/PipelineLayout.h"
#include "GraphicsModule/Core/Resource.h"

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

			inline const std::vector<Resource*>& GetResources() const
			{
				return m_Resources;
			}

			void SetResources(std::vector<Resource*>& resources) override;
			void SetResource(uint32 index, Resource* resource) override;
			uint32 GetNumBindings() const override;

		private:

			std::vector<Resource*> m_Resources;
			std::vector<BindingDescriptor> m_Bindings;
		};
	}
}