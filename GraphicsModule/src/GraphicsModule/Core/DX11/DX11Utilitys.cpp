#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{
		static const GUID g_WKPDID_D3DDebugObjectName = { 0x429b8c22, 0x9188, 0x4b0c, { 0x87,0x42,0xac,0xb0,0xbf,0x85,0xc2,0x00 } };

		void DX11SetObjectName(ID3D11DeviceChild* obj, const char* name)
		{
			if (obj != nullptr)
			{
				if (name != nullptr)
				{
					uint32 _length = static_cast<uint32>(std::strlen(name));

					obj->SetPrivateData(g_WKPDID_D3DDebugObjectName, _length, name);
				}
				else
				{
					obj->SetPrivateData(g_WKPDID_D3DDebugObjectName, 0, nullptr);
				}
			}
		}

		void DX11SetObjectNameSubscript(ID3D11DeviceChild* obj, const char* name, const char* subscript)
		{
			if (obj != nullptr)
			{
				if (name != nullptr)
				{
					std::string _nameWithSubscript = name;
					_nameWithSubscript += subscript;

					const size_t _length = _nameWithSubscript.size();

					obj->SetPrivateData(g_WKPDID_D3DDebugObjectName, static_cast<UINT>(_length), _nameWithSubscript.c_str());
				}
				else
				{
					obj->SetPrivateData(g_WKPDID_D3DDebugObjectName, 0, nullptr);
				}
			}
		}

		void DX11SetObjectNameIndexed(ID3D11DeviceChild* obj, const char* name, uint32 idx)
		{
			if (name != nullptr)
			{
				std::string _subscript = std::to_string(idx);

				DX11SetObjectNameSubscript(obj, name, _subscript.c_str());
			}
			else
			{
				DX11SetObjectName(obj, nullptr);
			}
		}

	}
}