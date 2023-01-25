#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Buffer.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11ResourceFlags.h"

#include "GraphicsModule/Core/DX11/DX11Type.h"
#include "GraphicsModule/Utility/Helper.h"
namespace Graphics
{
	namespace DX11
	{

		DX11Buffer::DX11Buffer(ID3D11Device* device, const BufferDesc& desc, const void* initData)
			: Buffer(desc._bindFlags)
			, m_BufferDesc(desc)
			, m_Size(desc._size)
			, m_Stride(desc._stride)
			, m_Format(MapFormat(desc._format))
		{
			CreateBuffer(device, desc, initData);
		}

		DX11Buffer::~DX11Buffer()
		{

		}

		void DX11Buffer::UpdateSubresource(ID3D11DeviceContext* context, const void* data, uint32 dataSize, uint32 offset)
		{
			if (GetDXUsage() == D3D11_USAGE_DYNAMIC)
			{
				D3D11_MAPPED_SUBRESOURCE _mappedSubresource;
				ZeroMemory(&_mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

				context->Map(GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &_mappedSubresource);

				memcpy(reinterpret_cast<char*>(_mappedSubresource.pData) + offset, data, dataSize);

				context->Unmap(GetBuffer(), 0);
			}
			else
			{
				if ((m_BufferDesc._bindFlags & BindFlags::ConstantBuffer) != 0)
				{
					if (dataSize == GetSize())
					{
						context->UpdateSubresource(GetBuffer(), 0, nullptr, data, 0, 0);
					}
					else
					{
						AssertMessageBox(false, "DX11Buffer UpdateSubresource Buffer Size Over");
					}
				}
				else
				{
					const D3D11_BOX _dstBox{ offset, 0, 0, offset + dataSize, 1, 1 };
					context->UpdateSubresource(GetBuffer(), 0, &_dstBox, data, 0, 0);
				}
			}
		}


		void DX11Buffer::ReadSubresource(ID3D11DeviceContext* context, void* data, uint32 dataSize, uint32 offset)
		{
			// Todo :: 리소스를 읽어 오는 부분 필요
			D3D11_MAPPED_SUBRESOURCE _subResouce;
		
			assert(false);
		}

		D3D11_MAP DX11Buffer::GetCPUAccessTypeForUsage(const CPUAccess access) const
		{
			if (access == CPUAccess::WriteDiscard && m_Usage != D3D11_USAGE_DYNAMIC)
				return D3D11_MAP_WRITE;
			else
				return D3D11_MAP_READ;
		}

		void* DX11Buffer::Map(ID3D11DeviceContext* context, const CPUAccess access, uint32 offset, uint32 size)
		{
			HRESULT hr = 0;
			D3D11_MAPPED_SUBRESOURCE _mappedSubresource;

			hr = context->Map(GetBuffer(), 0, GetCPUAccessTypeForUsage(access), 0, &_mappedSubresource);

			return (SUCCEEDED(hr) ? _mappedSubresource.pData : nullptr);
		}

		void DX11Buffer::Unmap(ID3D11DeviceContext* context)
		{
			context->Unmap(GetBuffer(), 0);
		}

		static uint32 GetD3DBufferSize(const BufferDesc& desc)
		{
			auto size = static_cast<uint32>(desc._size);
			if ((desc._bindFlags & BindFlags::ConstantBuffer) != 0)
				return GetAlignedSize(size, 16u);
			else
				return size;
		}

		void DX11Buffer::CreateBuffer(ID3D11Device* device, const BufferDesc& desc, const void* initData /*= nullptr*/)
		{
			D3D11_BUFFER_DESC _desc;
			{
				_desc.ByteWidth = GetD3DBufferSize(desc);
				_desc.Usage = DXGetBufferUsage(desc);
				_desc.BindFlags = DXGetBufferBindFlags(desc._bindFlags);
				_desc.CPUAccessFlags = DXGetCPUAccessFlagsForMiscFlags(desc._miscFlags);
				_desc.MiscFlags = DXGetBufferMiscFlags(desc);
				_desc.StructureByteStride = desc._stride;
			}

			m_Usage = _desc.Usage;

			if (initData)
			{
				D3D11_SUBRESOURCE_DATA subresourceData;
				{
					subresourceData.pSysMem = initData;
					subresourceData.SysMemPitch = 0;
					subresourceData.SysMemSlicePitch = 0;
				}

				device->CreateBuffer(&_desc, &subresourceData, m_Buffer.ReleaseAndGetAddressOf());
			}
			else
			{
				device->CreateBuffer(&_desc, nullptr, m_Buffer.ReleaseAndGetAddressOf());
			}
		}
	}
}
