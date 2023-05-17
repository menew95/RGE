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

			inline const std::vector<void*>& GetResources() const
			{
				return m_Resources;
			}

			void SetResources(std::vector<void*>& resources) override;
			void SetResource(uint32 index, void* resource) override;
			uint32 GetNumBindings() const override;
			void* GetResource(uint32 index) override;

			Buffer* GetBuffer(uint32 index) override;
			Texture* GetTexture(uint32 index) override;
			Sampler* GetSampler(uint32 index) override;

		private:
			std::vector<void*> m_Resources;
			std::vector<BindingDescriptor> m_Bindings;
		};
	}
}