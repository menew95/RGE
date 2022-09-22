#pragma once

#include "Buffer.h"

namespace RenderCore
{
	class DeviceContext;

	class ConstBuffer : public Buffer
	{
	public:
		ConstBuffer();
		~ConstBuffer();

		HRESULT Update(DeviceContext& context, const void* data, size_t byteCount);
	};
}