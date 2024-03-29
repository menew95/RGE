#include "GraphicsPCH.h"
#include "GraphicsModule/Core/BufferFlags.h"

namespace Graphics
{

	GRAPHICS_DLL_DECLSPEC bool IsTypedBuffer(const BufferDesc& desc)
	{
		return
			(
				desc._stride == 0 &&
				desc._format != Format::UNKNOWN &&
				(desc._bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0
				);
	}

	GRAPHICS_DLL_DECLSPEC bool IsStructuredBuffer(const BufferDesc& desc)
	{
		return
			(
				desc._stride > 0 &&
				(desc._bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0
				);
	}

	GRAPHICS_DLL_DECLSPEC bool IsByteAddressBuffer(const BufferDesc& desc)
	{
		return
			(
				desc._stride == 0 &&
				desc._format == Format::UNKNOWN &&
				(desc._bindFlags & (BindFlags::ShaderResource | BindFlags::UnorderedAccess)) != 0
				);
	}

}