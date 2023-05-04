#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11BufferRV.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"
#include "GraphicsModule/Core/BufferUtils.h""
#include "GraphicsModule/Core/BufferFlags.h"
#include <algorithm>

namespace Graphics
{
	static DXGI_FORMAT GetD3DResourceViewFormat(const BufferDesc& desc)
	{
		if (IsTypedBuffer(desc))
			return DX11::MapFormat(desc._format);
		if (IsByteAddressBuffer(desc))
			return DXGI_FORMAT_R32_TYPELESS;
		return DXGI_FORMAT_UNKNOWN;
	}

	static uint32 GetUAVFlags(const BufferDesc& desc)
	{
		if ((desc._bindFlags & BindFlags::UnorderedAccess) != 0)
		{
			if (IsStructuredBuffer(desc))
			{
				if ((desc._miscFlags & MiscFlags::Append) != 0)
					return D3D11_BUFFER_UAV_FLAG_APPEND;
				if ((desc._miscFlags & MiscFlags::Counter) != 0)
					return D3D11_BUFFER_UAV_FLAG_COUNTER;
			}
			else if (IsByteAddressBuffer(desc))
				return D3D11_BUFFER_UAV_FLAG_RAW;
		}
		return 0;
	}

	namespace DX11
	{

		DX11BufferRV::DX11BufferRV(ID3D11Device* device, const BufferDesc& desc, const void* initialData /*= nullptr*/)
			: DX11Buffer(device, desc, initialData)
			, m_UAVFlags { GetUAVFlags(desc) }
		{
			const uint32 _stride = GetStorageBufferStride(desc);

			auto _format = GetD3DResourceViewFormat(desc);
			auto _numElements = static_cast<uint32>(desc._size) / _stride;

			if ((desc._bindFlags & BindFlags::ShaderResource) != 0)
				CreateInternalSRV(device, _format, 0, _numElements);

			if ((desc._bindFlags & BindFlags::UnorderedAccess) != 0)
				CreateInternalUAV(device, _format, 0, _numElements);
		}

		static void CreateD3D11BufferSubresourceSRV(
			ID3D11Device* device,
			ID3D11Resource* resource,
			ID3D11ShaderResourceView** srvOutput,
			DXGI_FORMAT format,
			uint32 firstElement,
			uint32 numElements,
			const char* errorContextInfo = nullptr)
		{
			/* Initialize descriptor and create SRV */
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			{
				desc.Format = format;
				desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = firstElement;
				desc.Buffer.NumElements = numElements;
			}
			auto hr = device->CreateShaderResourceView(resource, &desc, srvOutput);
		}

		void DX11BufferRV::CreateSubresourceSRV(ID3D11Device* device, ID3D11ShaderResourceView** srvOutput, DXGI_FORMAT format, UINT firstElement, UINT numElements)
		{
			CreateD3D11BufferSubresourceSRV(
				device,
				GetNative(),
				srvOutput,
				format,
				firstElement,
				numElements,
				"for buffer subresource"
			);
		}

		static void CreateD3D11BufferSubresourceUAV(
			ID3D11Device* device,
			ID3D11Resource* resource,
			ID3D11UnorderedAccessView** uavOutput,
			DXGI_FORMAT format,
			uint32 firstElement,
			uint32 numElements,
			uint32 flags,
			const char* errorContextInfo = nullptr)
		{
			/* Initialize descriptor and create UAV */
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
			{
				desc.Format = format;
				desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = firstElement;
				desc.Buffer.NumElements = numElements;
				desc.Buffer.Flags = flags;
			}
			auto hr = device->CreateUnorderedAccessView(resource, &desc, uavOutput);
		}

		void DX11BufferRV::CreateSubresourceUAV(ID3D11Device* device, ID3D11UnorderedAccessView** uavOutput, DXGI_FORMAT format, UINT firstElement, UINT numElements)
		{
			CreateD3D11BufferSubresourceUAV(
				device,
				GetNative(),
				uavOutput,
				format,
				firstElement,
				numElements,
				m_UAVFlags,
				"for buffer subresource"
			);
		}

		void DX11BufferRV::SetName(const char* name)
		{
			DX11Buffer::SetName(name);

			if (m_SRV)
			{
				DX11SetObjectNameSubscript(m_SRV.Get(), name, ".SRV");
			}
			if (m_UAV)
			{
				DX11SetObjectNameSubscript(m_UAV.Get(), name, ".UAV");
			}
		}

		// private

		void DX11BufferRV::CreateInternalSRV(ID3D11Device* device, DXGI_FORMAT format, UINT firstElement, UINT numElements)
		{
			CreateD3D11BufferSubresourceSRV(
				device,
				GetNative(),
				m_SRV.ReleaseAndGetAddressOf(),
				format,
				firstElement,
				numElements,
				"for buffer"
			);
		}

		void DX11BufferRV::CreateInternalUAV(ID3D11Device* device, DXGI_FORMAT format, UINT firstElement, UINT numElements)
		{
			CreateD3D11BufferSubresourceUAV(
				device,
				GetNative(),
				m_UAV.ReleaseAndGetAddressOf(),
				format,
				firstElement,
				numElements,
				m_UAVFlags,
				"for buffer"
			);
		}

	}
}