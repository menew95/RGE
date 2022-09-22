#include "../../pch.h"
#include "IncludeDX11.h"
#include "Buffer.h"

RenderCore::Buffer::Buffer()
: mBuffer(nullptr)
, mBufferSize(0)
, mBufferType(eBufferType::Unknown)
{

}

RenderCore::Buffer::Buffer(eBufferType type)
: mBuffer(nullptr)
, mBufferSize(0)
, mBufferType(type)
{

}
