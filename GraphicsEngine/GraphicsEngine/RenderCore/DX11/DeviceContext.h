#pragma once

#include "../ID3D.h"

namespace RenderCore
{
	class Device;
	class VertexBuffer;
	class IndexBuffer;
	class InputLayout;
	class VertexShader;
	class GeometryShader;
	class PixelShader;
	class ComputeShader;
	class DomainShader;
	class HullShader;
	class RasterizerState;
	class BlendState;
	class DepthStencilState;
	class ViewportDesc;

	class DeviceContext : public std::enable_shared_from_this<DeviceContext>
	{
	public:
		DeviceContext();
		~DeviceContext();


		ID3D::DeviceContext* GetDeviceContext() const { return mDeviceContext.Get(); }
		ID3D::DeviceContext** GetDeviceContextRef() { return mDeviceContext.GetAddressOf(); }
	
		void Bind(uint32 bindPoint, uint32 count, const VertexBuffer* VBs[], const uint32 strides[], const uint32 offsets[]);
		void Bind(const InputLayout& inputLayout);
		void Bind(const IndexBuffer& IB, DXGI_FORMAT format, uint32 offset);
		void Bind(const VertexShader& vertexShader);
		void Bind(const GeometryShader& geometryShader);
		void Bind(const PixelShader& pixelShader);
		void Bind(const ComputeShader& computeShader);
		void Bind(const DomainShader& domainShader);
		void Bind(const HullShader& hullShader);
		void Bind(const RasterizerState& rasterizer);
		void Bind(const BlendState& blender);
		void Bind(const DepthStencilState& depthStencilState, uint32 stencilRef = 0x0);
		void Bind(const ViewportDesc& viewport);

	private:
		ComPtr<ID3D::DeviceContext> mDeviceContext;
	};
}
