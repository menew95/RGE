#pragma once

#include "IResource.h"

namespace RenderCore
{
	enum class eBufferType
	{
		VertexBuffer = 0,
		IndexBuffer = 1,
		ConstBuffer = 2,
		Unknown = 3
	};

	class Buffer : public IResource<Buffer>
	{
	public:
		Buffer();
		Buffer(eBufferType type);
		virtual ~Buffer() {}

		ID3D::Buffer* GetBuffer() const { return mBuffer.Get(); }
		ID3D::Buffer* const* GetBufferRef() { return mBuffer.GetAddressOf(); }
		
		uint32 GetBufferSize() const { return mBufferSize; }
		eBufferType GetType() const { return mBufferType; }

	private:
		ComPtr<ID3D::Buffer> mBuffer;

		uint32 mBufferSize;

		eBufferType mBufferType;
	};
}