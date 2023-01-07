#pragma once

struct ID3D11DeviceChild;

namespace Graphics
{
	namespace DX11
	{
		void DX11ObjectSetName(ID3D11DeviceChild* obj, const char* name);
	}
}