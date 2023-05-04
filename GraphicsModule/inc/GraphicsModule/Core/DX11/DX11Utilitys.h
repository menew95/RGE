#pragma once

struct ID3D11DeviceChild;

namespace Graphics
{
	namespace DX11
	{
		void DX11SetObjectName(ID3D11DeviceChild* obj, const char* name);

		void DX11SetObjectNameSubscript(ID3D11DeviceChild* obj, const char* name, const char* subscript);

		void DX11SetObjectNameIndexed(ID3D11DeviceChild* obj, const char* name, uint32 idx);
	}
}