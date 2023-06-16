#pragma once

#include "Struct/VertexAttribute.h"
//#include <LLGL/PipelineStateFlags.h>
//#include <LLGL/SwapChainFlags.h>
//#include <LLGL/SamplerFlags.h>
//#include <LLGL/TextureFlags.h>
//#include <LLGL/BufferFlags.h>
//#include <LLGL/QueryHeapFlags.h>
#include "GraphicsModule/Core/SwapChainFlags.h"
#include "GraphicsModule/Core/PipelineStateFlags.h"
#include "GraphicsModule/Core/TextureFlags.h"
#include "GraphicsModule/Core/BufferFlags.h"
#include "GraphicsModule/Core/SamplerFlags.h"
#include "GraphicsModule/Core/RenderSystemFlags.h"
#include "Direct3D11.h"


namespace Graphics
{
	namespace DX11
	{
		DXGI_FORMAT                 MapFormat(const DataType dataType);
		DXGI_FORMAT                 MapFormat(const Format format);
		D3D_PRIMITIVE_TOPOLOGY      MapPrimitive(const PrimitiveTopology topology);
		D3D11_FILL_MODE             MapFillMode(const FillMode polygonMode);
		D3D11_CULL_MODE             MapCullMode(const CullMode cullMode);
		D3D11_BLEND                 MapBlend(const Blend blend);
		D3D11_BLEND_OP              MapBlendOp(const BlendOp blendOp);
		D3D11_COMPARISON_FUNC       MapCompareOp(const CompareOp compareOp);
		D3D11_STENCIL_OP            MapStencilOp(const StencilOp stencilOp);
		D3D11_FILTER                MapFilter(const Filter samplerDesc);
		D3D11_TEXTURE_ADDRESS_MODE  MapSamplerMode(const SamplerAddressMode	addressMode);
		D3D11_MAP                   MapCpuFlag(const CPUAccess cpuAccess);

		uint32						MapBufferBindFlag(const uint32 bindFlags);
		uint32						MapTextureBindFlag(const uint32 bindFlags);
		uint32						MapCPUAcessFlagsForMiscFlags(const uint32 miscFlags);
		Format                      UnmapFormat(const DXGI_FORMAT format);

		//TODO: remove and use DXTypes namespace directly
		DXGI_FORMAT                 ToDXGIFormatDSV(const DXGI_FORMAT format);
		DXGI_FORMAT                 ToDXGIFormatSRV(const DXGI_FORMAT format);
		DXGI_FORMAT                 ToDXGIFormatUAV(const DXGI_FORMAT format);
		DXGI_FORMAT					ToDXGIFormatUINT(const DXGI_FORMAT format);

		void Convert(D3D11_DEPTH_STENCIL_DESC& dst, const DepthDesc& srcDepth, const StencilDesc& srcStencil);

		void Convert(D3D11_RASTERIZER_DESC& dst, const RasterizerDesc& src);

		void Convert(D3D11_BLEND_DESC& dst, const BlendDesc& src);

	}
}