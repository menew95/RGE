#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11ResourceView.h"
#include "GraphicsModule/Core/DX11/DX11Texture.h"
#include "GraphicsModule/Core/ShaderFlags.h"

namespace Graphics
{
	namespace DX11
	{

		DX11ResourceView::DX11ResourceView(ID3D11Device* device, const ResourceViewDesc& desc)
		{

		}

		void DX11ResourceView::BindForGraphicsPipeline(ID3D11DeviceContext* context, uint32 slot, uint32 count, long bindFlags, long stageFlags)
		{
			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				std::vector<ID3D11ShaderResourceView*> _srvs(count, nullptr);

				for (uint32 i = 0; i < count; i++)
				{
					_srvs[i] = m_SRVs[i].Get();
				}

				if((stageFlags & StageFlags::VS) != 0) context->VSSetShaderResources(slot, count, _srvs.data());
				if((stageFlags & StageFlags::HS) != 0) context->HSSetShaderResources(slot, count, _srvs.data());
				if((stageFlags & StageFlags::GS) != 0) context->GSSetShaderResources(slot, count, _srvs.data());
				if((stageFlags & StageFlags::DS) != 0) context->DSSetShaderResources(slot, count, _srvs.data());
				if((stageFlags & StageFlags::PS) != 0) context->PSSetShaderResources(slot, count, _srvs.data());
			}

			if ((bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				std::vector<ID3D11UnorderedAccessView*> _uavs(count, nullptr);
				std::vector<UINT> _auvCounts(count, 0);

				for (uint32 i = 0; i < count; i++)
				{
					_uavs[i] = m_UAVs[i].Get();
				}

				if ((stageFlags & StageFlags::PS) != 0) 
					context->OMSetRenderTargetsAndUnorderedAccessViews(
						D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
						nullptr,
						nullptr,
						slot,
						count,
						_uavs.data(),
						_auvCounts.data());
			}
		}

		void DX11ResourceView::BindForComputePipeline(ID3D11DeviceContext* context, uint32 slot, uint32 count, long bindFlags, long stageFlags)
		{
			if ((bindFlags & BindFlags::ShaderResource) != 0)
			{
				std::vector<ID3D11ShaderResourceView*> _srvs(count, nullptr);

				for (uint32 i = 0; i < count; i++)
				{
					_srvs[i] = m_SRVs[i].Get();
				}

				context->CSSetShaderResources(slot, count, _srvs.data());
			}

			if ((bindFlags & BindFlags::UnorderedAccess) != 0)
			{
				std::vector<ID3D11UnorderedAccessView*> _uavs(count, nullptr);
				std::vector<UINT> _auvCounts(count, 0);

				for (uint32 i = 0; i < count; i++)
				{
					_uavs[i] = m_UAVs[i].Get();
				}

				context->CSSetUnorderedAccessViews(slot, count, _uavs.data(), _auvCounts.data());
			}
		}

		// private
		void DX11ResourceView::CreateUnorderedAccessView(const ResourceViewDesc& desc)
		{
			for (size_t i = 0; i < desc._texSubresources.size(); i++)
			{
				DX11Texture& _castTexture = reinterpret_cast<DX11Texture&>(*(desc._resources[i]));

				GetOrCreateUAV(_castTexture, desc._texSubresources[i]);
			}
		}

		void DX11ResourceView::CreateShaderAccessView(const ResourceViewDesc& desc)
		{
			for (size_t i = 0; i < desc._texSubresources.size(); i++)
			{
				DX11Texture& _castTexture = reinterpret_cast<DX11Texture&>(*(desc._resources[i]));

				GetOrCreateSRV(_castTexture, desc._texSubresources[i]);
			}
		}

		uint PackTextureSubresource(const TextureSubresource& subresource)
		{
			uint32 _pack = subresource.baseArrayLayer << 24 |
				subresource.numArrayLayers << 16 |
				subresource.baseMipLevel << 8 |
				subresource.numMipLevels;

			return _pack;
		}

		TextureSubresource UnPackTextureSubresource(uint32 pack)
		{
			TextureSubresource _unPack
			{
				_unPack.baseArrayLayer = (pack && 0xff000000) >> 24,
				_unPack.numArrayLayers = (pack && 0x00ff0000) >> 16,
				_unPack.baseMipLevel = (pack && 0x0000ff00) >> 8,
				_unPack.numMipLevels = (pack && 0x000000ff)
			};

			return _unPack;
		}

		ID3D11ShaderResourceView* DX11ResourceView::GetOrCreateSRV(DX11Texture& texture, const TextureSubresource& subresource)
		{
			uint32 _pack = PackTextureSubresource(subresource);

			if (subresource.numMipLevels > 0 &&
				subresource.numArrayLayers > 0)
			{
				ComPtr<ID3D11Device> _device;
				texture.GetNativeTexture()._resource->GetDevice(_device.GetAddressOf());

				ComPtr<ID3D11ShaderResourceView> _srv;
				texture.CreateSubresourceSRV(
					_device.Get(),
					_srv.GetAddressOf(),
					subresource.baseMipLevel,
					subresource.numMipLevels,
					subresource.baseArrayLayer,
					subresource.numArrayLayers);

				m_SRVs.push_back(std::move(_srv));

				return m_SRVs.back().Get();
			}
			else
			{
				return texture.GetSRV();
			}
		}

		ID3D11UnorderedAccessView* DX11ResourceView::GetOrCreateUAV(DX11Texture& texture, const TextureSubresource& subresource)
		{
			uint32 _pack = PackTextureSubresource(subresource);

			if (subresource.numMipLevels > 0 &&
				subresource.numArrayLayers > 0)
			{
				ComPtr<ID3D11Device> _device;
				texture.GetNativeTexture()._resource->GetDevice(_device.GetAddressOf());

				ComPtr<ID3D11UnorderedAccessView> _uav;
				texture.CreateSubresourceUAV(
					_device.Get(),
					_uav.GetAddressOf(),
					subresource.baseMipLevel,
					subresource.numMipLevels,
					subresource.baseArrayLayer,
					subresource.numArrayLayers);

				m_UAVs.push_back(std::move(_uav));

				return m_UAVs.back().Get();
			}
			else
			{
				return texture.GetUAV();
			}
		}

	}
}