#include "GraphicsPCH.h"
#include "GraphicsModule/Core/BufferFlags.h"

namespace Graphics
{

	GRAPHICS_DLL_DECLSPEC bool IsStructuredBuffer(const BufferDesc& desc)
	{
		return
			(
				desc._stride > 0 &&
				(desc._bindFlags & (BindFlags::Sampled | BindFlags::Storage)) != 0
				);
	}

	GRAPHICS_DLL_DECLSPEC bool IsByteAddressBuffer(const BufferDesc& desc)
	{
		return
			(
				desc._stride == 0 &&
				desc._format == Format::UNKNOWN &&
				(desc._bindFlags & (BindFlags::Sampled | BindFlags::Storage)) != 0
				);
	}

}