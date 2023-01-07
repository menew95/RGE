#pragma once

#include "Common.h"
#include "Struct/VertexAttribute.h"
#include "GraphicsModule/Core/Format.h"
#include "GraphicsModule/Core/ResourceFlags.h"
namespace Graphics
{
	struct BUFFER_DESC
	{
		uint32 _byteWidth;
		uint32 _usage;
		uint32 _bindFlags;
		uint32 _cpuAccessFlags;
		uint32 _miscFlags;
		uint32 _structureByteStride;
	};

	struct BufferDesc
	{
		uint32 _size = 0;
		uint32 _stride = 0;
		uint32 _bindFlags = 0;
		uint32 _cpuAccessFlags = 0;
		uint32 _slot = 0;
		uint32 _miscFlags = 0;
		Format _format;

		std::vector<VertexAttribute>  _vertexAttribs;
	};

	GRAPHICS_DLL_DECLSPEC bool IsStructuredBuffer(const BufferDesc& desc);
	GRAPHICS_DLL_DECLSPEC bool IsByteAddressBuffer(const BufferDesc& desc);
}