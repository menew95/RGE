#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Sampler.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"
#include "GraphicsModule/Core/DX11/DX11Type.h"

namespace Graphics
{
	namespace DX11
	{

		DX11Sampler::DX11Sampler(ID3D11Device* device, const SamplerDesc& desc)
		{
			D3D11_SAMPLER_DESC _desc;
			{
				_desc.Filter	= MapFormat(desc._filter);
				_desc.AddressU	= MapFormat(desc._addressU);
				_desc.AddressV	= MapFormat(desc._addressV);
				_desc.AddressW	= MapFormat(desc._addressW);
				_desc.MipLODBias = desc._mipLODBias;
				_desc.MaxAnisotropy = desc._maxAnisotropy;
				_desc.ComparisonFunc = MapFormat(desc._comparisonOp);
				_desc.BorderColor[0] = desc._borderColor.x;
				_desc.BorderColor[1] = desc._borderColor.y;
				_desc.BorderColor[2] = desc._borderColor.z;
				_desc.BorderColor[3] = desc._borderColor.w;

				if (desc._mipMapping)
				{
					_desc.MinLOD = desc._minLOD;
					_desc.MaxLOD = desc._maxLOD;
				}
				else
				{
					_desc.MinLOD = 0.0f;
					_desc.MaxLOD = 0.0f;
				}
			}

			HR(device->CreateSamplerState(&_desc, m_SamplerState.ReleaseAndGetAddressOf()));
		}

		void DX11Sampler::SetName(const char* name)
		{
			DX11ObjectSetName(m_SamplerState.Get(), name);
		}

	}
}