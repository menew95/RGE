#include "GraphicsPCH.h"
#include "GraphicsModule/Core/DX11/DX11Utilitys.h"
#include "GraphicsModule/Core/DX11/Direct3D11.h"

namespace Graphics
{
	namespace DX11
	{
		static const GUID g_WKPDID_D3DDebugObjectName = { 0x429b8c22, 0x9188, 0x4b0c, { 0x87,0x42,0xac,0xb0,0xbf,0x85,0xc2,0x00 } };

		void DX11ObjectSetName(ID3D11DeviceChild* obj, const char* name)
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

	}
}