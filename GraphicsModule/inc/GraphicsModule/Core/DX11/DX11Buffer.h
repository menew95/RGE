#pragma once

#include "GraphicsModule/Core/Buffer.h"
#include "GraphicsModule/Core/Common/ComPtr.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/RenderSystemFlags.h"

struct ID3D11Device;
struct ID3D11Buffer;

namespace Graphics
{
	namespace DX11
	{
		class DX11Buffer : public Buffer
		{
		public:
			DX11Buffer() = default;
			DX11Buffer(ID3D11Device* device, const BufferDesc& desc, const void* initData);
			~DX11Buffer() override;

			void UpdateSubresource(ID3D11DeviceContext* context, const void* data, uint32 dataSize, uint32 offset);
			void ReadSubresource(ID3D11DeviceContext* context, void* data, uint32 dataSize, uint32 offset);

			void* Map(ID3D11DeviceContext* context, const CPUAccess access, uint32 offset, uint32 size);
			void Unmap(ID3D11DeviceContext* context);

			
			inline ID3D11Buffer* GetBuffer() const { return m_Buffer.Get(); }
			inline ID3D11Buffer* const* GetBufferRef() const { return m_Buffer.GetAddressOf(); }

			inline ID3D11Buffer* GetNative() const
			{
				return m_Buffer.Get();
			}

			inline uint32 GetSize() const
			{
				return m_Size;
			}

			inline uint32 GetStride() const
			{
				return m_Stride;
			}

			inline DXGI_FORMAT GetDXFormat() const
			{
				return m_Format;
			}

			inline D3D11_USAGE GetDXUsage() const
			{
				return m_Usage;
			}

			void SetName(const char* name) override;

		private:
			void CreateBuffer(ID3D11Device* device, const BufferDesc& desc, const void* initData = nullptr);
			
			D3D11_MAP GetCPUAccessTypeForUsage(const CPUAccess access) const;

			ComPtr<ID3D11Buffer>	m_Buffer;

			BufferDesc				m_BufferDesc;

			uint32					m_Size = 0;
			uint32					m_Stride = 0;
			DXGI_FORMAT				m_Format = DXGI_FORMAT_UNKNOWN;
			D3D11_USAGE				m_Usage = D3D11_USAGE_DEFAULT;
			uint32					mappedWriteRange_[2] = { 0, 0 };
		};
	}
}