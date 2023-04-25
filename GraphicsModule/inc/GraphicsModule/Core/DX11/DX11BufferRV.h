#pragma once

#include "GraphicsModule/Core/DX11/DX11Buffer.h"

namespace Graphics 
{
	namespace DX11
	{
		class DX11BufferRV : public DX11Buffer
		{
		public:
			DX11BufferRV(ID3D11Device* device, const BufferDesc& desc, const void* initialData = nullptr);

			// Creates a shader-resource-view (SRV) of a subresource of this buffer object.
			void CreateSubresourceSRV(
				ID3D11Device* device,
				ID3D11ShaderResourceView** srvOutput,
				DXGI_FORMAT                 format,
				UINT                        firstElement,
				UINT                        numElements
			);

			// Creates an unordered-access-view (UAV) of a subresource of this buffer object.
			void CreateSubresourceUAV(
				ID3D11Device* device,
				ID3D11UnorderedAccessView** uavOutput,
				DXGI_FORMAT                 format,
				UINT                        firstElement,
				UINT                        numElements
			);

			// Returns the native SRV object.
			inline ID3D11ShaderResourceView* GetSRV() const
			{
				return srv_.Get();
			}

			// Returns the native UAV object.
			inline ID3D11UnorderedAccessView* GetUAV() const
			{
				return uav_.Get();
			}

			// Returns the initial value for the internal buffer counter.
			inline UINT GetInitialCount() const
			{
				return initialCount_;
			}

		private:

			void CreateInternalSRV(ID3D11Device* device, DXGI_FORMAT format, UINT firstElement, UINT numElements);
			void CreateInternalUAV(ID3D11Device* device, DXGI_FORMAT format, UINT firstElement, UINT numElements);

		private:

			ComPtr<ID3D11ShaderResourceView>    srv_;
			ComPtr<ID3D11UnorderedAccessView>   uav_;
			UINT                                uavFlags_ = 0;
			UINT                                initialCount_ = UINT_MAX;
		};
	}
}