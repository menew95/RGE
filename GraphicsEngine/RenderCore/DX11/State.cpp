#include "../../pch.h"
#include "IncludeDX11.h"
#include "State.h"
#include "DeviceContext.h"
#include "ObjectFactory.h"

RenderCore::DepthStencilState::DepthStencilState(bool enabled /*= true*/, bool writeEnabled /*= true*/, Comparison::Enum comparison /*= Comparison::LessEqual*/)
{

}

RenderCore::DepthStencilState::DepthStencilState(bool depthTestEnabled, bool writeEnabled, unsigned stencilReadMask, unsigned stencilWriteMask, const StencilMode& frontFaceStencil /*= StencilMode::NoEffect*/, const StencilMode& backFaceStencil /*= StencilMode::NoEffect*/)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = depthTestEnabled | writeEnabled;
	desc.DepthWriteMask = writeEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = depthTestEnabled ? D3D11_COMPARISON_LESS_EQUAL : D3D11_COMPARISON_ALWAYS;
	desc.StencilEnable = true;
	desc.StencilReadMask = (UINT8)stencilReadMask;
	desc.StencilWriteMask = (UINT8)stencilWriteMask;
	desc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)frontFaceStencil._onStencilFail;
	desc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)frontFaceStencil._onDepthFail;
	desc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)frontFaceStencil._onPass;
	desc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)frontFaceStencil._comparison;
	desc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)backFaceStencil._onStencilFail;
	desc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)backFaceStencil._onDepthFail;
	desc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)backFaceStencil._onPass;
	desc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)backFaceStencil._comparison;

	//mState = ObjectFactory().CreateDepthStencilState(&desc);
}

RenderCore::ViewportDesc::ViewportDesc(ID3D::DeviceContext* context)
{
	if (context) {
		UINT viewportsToGet = 1;
		context->RSGetViewports(&viewportsToGet, (D3D11_VIEWPORT*)this);
	}
}

RenderCore::ViewportDesc::ViewportDesc(const DeviceContext& context)
{
	UINT viewportsToGet = 1;
	context.GetDeviceContext()->RSGetViewports(&viewportsToGet, (D3D11_VIEWPORT*)this);
}