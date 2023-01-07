#pragma once

#include "GraphicsModule/Core/Format.h"

namespace Graphics
{
	struct SrcImageDesc
	{
		SrcImageDesc() = default;
		SrcImageDesc(const SrcImageDesc&) = default;

		//! Constructor to initialize all attributes.
		inline SrcImageDesc(ImageFormat format, DataType dataType, const void* data, size_t dataSize) :
			format{ format },
			dataType{ dataType },
			data{ data },
			dataSize{ dataSize }
		{
		}

		//! Specifies the image format. By default ImageFormat::RGBA.
		ImageFormat format = ImageFormat::RGBA;

		//! Specifies the image data type. This must be DataType::UInt8 for compressed images. By default DataType::UInt8.
		DataType    dataType = DataType::UInt8;

		//! Pointer to the read-only image data.
		const void* data = nullptr;

		//! Specifies the size (in bytes) of the image data. This is primarily used for compressed images and serves for robustness.
		size_t dataSize = 0;
	};
}