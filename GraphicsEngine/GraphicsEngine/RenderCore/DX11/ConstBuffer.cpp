#include "../../pch.h"
#include "ConstBuffer.h"
#include "IncludeDX11.h"
#include "DeviceContext.h"

RenderCore::ConstBuffer::ConstBuffer()
	: Buffer(eBufferType::ConstBuffer)
{

}

RenderCore::ConstBuffer::~ConstBuffer()
{

}

HRESULT RenderCore::ConstBuffer::Update(DeviceContext& context, const void* data, size_t byteCount)
{
	D3D11_MAPPED_SUBRESOURCE result;
	ID3D::DeviceContext* devContext = context.GetDeviceContext();
	HRESULT hresult = devContext->Map(GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &result);
	if (SUCCEEDED(hresult) && result.pData) {
		memcpy(result.pData, data, byteCount);
		devContext->Unmap(GetBuffer(), 0);
	}
}
