#pragma once

#include "GraphicsModule\Core\Device.h"
#include "GraphicsModule\Core\Common\ComPtr.h"

struct ID3D11Device;

namespace Graphics
{
	namespace DX11 
	{
		class DX11Device final : public Device
		{
		public:
			DX11Device();
			~DX11Device() override;

			using UnderlyingType = ID3D11Device*;

			UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

		private:
			ComPtr<ID3D11Device> m_Underlying;
		};
	}
}