#include "../../pch.h"
#include "IncludeDX11.h"
#include "DeviceContext.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Shader.h"
#include "State.h"

RenderCore::DeviceContext::DeviceContext()
{

}

RenderCore::DeviceContext::~DeviceContext()
{

}

void RenderCore::DeviceContext::Bind(uint32 bindPoint, uint32 count, const VertexBuffer* VBs[], const uint32 strides[], const uint32 offsets[])
{
	ID3D::Buffer* buffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

	for (uint32 i = 0; i < count; ++i)
	{
		buffers[i] = VBs[i]->GetBuffer();
	}

	mDeviceContext->IASetVertexBuffers(bindPoint, count, buffers, strides, offsets);
}

void RenderCore::DeviceContext::Bind(const ViewportDesc& viewport)
{
	mDeviceContext->RSSetViewports(1, (D3D11_VIEWPORT*)&viewport);
}

void RenderCore::DeviceContext::Bind(const DepthStencilState& depthStencilState, uint32 stencilRef /*= 0x0*/)
{
	mDeviceContext->OMSetDepthStencilState(depthStencilState.GetState(), stencilRef);
}

void RenderCore::DeviceContext::Bind(const BlendState& blender)
{
	const FLOAT blendFactors[] = { 1.f, 1.f, 1.f, 1.f };
	mDeviceContext->OMSetBlendState(blender.GetState(), blendFactors, 0xffffffff);
}

void RenderCore::DeviceContext::Bind(const RasterizerState& rasterizer)
{
	mDeviceContext->RSSetState(rasterizer.GetState());
}

void RenderCore::DeviceContext::Bind(const HullShader& hullShader)
{
	mDeviceContext->HSSetShader(hullShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const DomainShader& domainShader)
{
	mDeviceContext->DSSetShader(domainShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const ComputeShader& computeShader)
{
	mDeviceContext->CSSetShader(computeShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const PixelShader& pixelShader)
{
	mDeviceContext->PSSetShader(pixelShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const GeometryShader& geometryShader)
{
	mDeviceContext->GSSetShader(geometryShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const VertexShader& vertexShader)
{
	mDeviceContext->VSSetShader(vertexShader.GetShader(), nullptr, 0);
}

void RenderCore::DeviceContext::Bind(const IndexBuffer& IB, DXGI_FORMAT format, uint32 offset)
{
	mDeviceContext->IASetIndexBuffer(IB.GetBuffer(), format, offset);
}

void RenderCore::DeviceContext::Bind(const InputLayout& inputLayout)
{
	mDeviceContext->IASetInputLayout(inputLayout.GetInputLayout());
}
