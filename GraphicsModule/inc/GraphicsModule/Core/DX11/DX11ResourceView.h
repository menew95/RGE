#include "GraphicsModule/Core/ResourceView.h"

#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{
		class DX11Texture;

		class DX11ResourceView : public ResourceView
		{
		public:
			DX11ResourceView(ID3D11Device* device, const ResourceViewDesc& desc);

			void BindForGraphicsPipeline(ID3D11DeviceContext* context, uint32 slot, uint32 count, long bindFlags, long stageFlags);
			void BindForComputePipeline(ID3D11DeviceContext* context, uint32 slot, uint32 count, long bindFlags, long stageFlags);

			void SetName(const char* name) override;

		private:
			void CreateUnorderedAccessView(const ResourceViewDesc& desc);
			void CreateShaderResourceView(const ResourceViewDesc& desc);

			ID3D11ShaderResourceView* GetOrCreateSRV(DX11Texture& texture, const TextureSubresource& subresource);
			ID3D11UnorderedAccessView* GetOrCreateUAV(DX11Texture& texture, const TextureSubresource& subresource);

			std::vector<ComPtr<ID3D11ShaderResourceView>> m_SRVs;
			std::vector<ComPtr<ID3D11UnorderedAccessView>> m_UAVs;
		};
	}
}