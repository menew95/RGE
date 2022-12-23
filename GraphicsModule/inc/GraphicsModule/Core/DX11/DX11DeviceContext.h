#pragma once

#include "GraphicsModule/Core/DeviceContext.h"
#include "GraphicsModule/Core/Common/ComPtr.h"

struct ID3D11DeviceContext;

namespace Graphics
{
	namespace DX11
	{
		class DX11Device;

		class DX11DeviceContext : public DeviceContext
		{
		public:
			DX11DeviceContext(DX11Device*);
			~DX11DeviceContext();


			using UnderlyingType = ID3D11DeviceContext*;

			UnderlyingType GetUnderlying() { return m_Underlying.Get(); }
			UnderlyingType* GetUnderlyingAddress() { return m_Underlying.GetAddressOf(); }

		private:
			ComPtr<ID3D11DeviceContext> m_Underlying;
		};
	}
}