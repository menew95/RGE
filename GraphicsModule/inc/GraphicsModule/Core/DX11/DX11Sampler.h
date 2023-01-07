#pragma once

#include "GraphicsModule/Core/Sampler.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

struct ID3D11SamplerState;
struct ID3D11Device;

namespace Graphics
{
	namespace DX11
	{
		class DX11Sampler : public Sampler
		{
		public:
			DX11Sampler(ID3D11Device* device, const SamplerDesc& desc);
			void SetName(const char* name) override;

			inline ID3D11SamplerState* GetSamplerState()
			{
				return m_SamplerState.Get();
			}

		private:
			ComPtr<ID3D11SamplerState> m_SamplerState;
		};
	}
}