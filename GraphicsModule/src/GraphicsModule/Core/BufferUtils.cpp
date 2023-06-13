#include "GraphicsPCH.h"
#include "GraphicsModule/Core/BufferUtils.h"
#include "GraphicsModule/Core/Buffer.h"

namespace Graphics
{

	GRAPHICS_DLL_DECLSPEC uint32 GetStorageBufferStride(const BufferDesc& desc)
	{
		if (desc._stride > 0)
			return desc._stride;
		else return 1;
	}

	GRAPHICS_DLL_DECLSPEC long GetCombinedBindFlags(uint32 numBuffers, Buffer* const* bufferArray)
	{
		long _bindFlags = 0;

		for (uint32 i = 0; i < numBuffers; ++i)
			_bindFlags |= bufferArray[i]->GetBindFlags();
		return _bindFlags;
	}

}