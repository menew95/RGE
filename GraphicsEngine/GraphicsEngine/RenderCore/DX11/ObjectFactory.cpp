#include "../../pch.h"
#include "IncludeDX11.h"
#include "ObjectFactory.h"
#include "Device.h"

static RenderCore::ObjectFactory* sObjectFactory = nullptr;

RenderCore::ObjectFactory::ObjectFactory(Device* device)
	:
	mDevice(device->GetDevice())
{
	assert(sObjectFactory == nullptr);

	sObjectFactory = this;
}

RenderCore::ObjectFactory::ObjectFactory(ID3D::Device* device)
	:
	mDevice(device)
{
	assert(sObjectFactory == nullptr);

	sObjectFactory = this;
}

RenderCore::ObjectFactory::~ObjectFactory()
{

}

ComPtr<ID3D::BlendState> RenderCore::ObjectFactory::CreateBlendState(const D3D11_BLEND_DESC* desc) const
{
	ComPtr<ID3D::BlendState> _state;
	
	HR(mDevice->CreateBlendState(desc, _state.GetAddressOf()));

	return std::move(_state);
}

ComPtr<ID3D::DepthStencilState> RenderCore::ObjectFactory::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* desc) const
{
	ComPtr<ID3D::DepthStencilState> _state;
	
	HR(mDevice->CreateDepthStencilState(desc, _state.GetAddressOf()));

	return _state;
}

ComPtr<ID3D::RasterizerState> RenderCore::ObjectFactory::CreateRasterizerState(const D3D11_RASTERIZER_DESC* desc) const
{
	ComPtr<ID3D::RasterizerState> _state;
	
	HR(mDevice->CreateRasterizerState(desc, _state.GetAddressOf()));

	return _state;
}

ComPtr<ID3D::SamplerState> RenderCore::ObjectFactory::CreateSamplerState(const D3D11_SAMPLER_DESC* desc) const
{
	ComPtr<ID3D::SamplerState> _state;

	HR(mDevice->CreateSamplerState(desc, _state.GetAddressOf()));

	return _state;
}

Microsoft::WRL::ComPtr<ID3D::Buffer> RenderCore::ObjectFactory::CreateBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* data /*= nullptr*/, const char name[] /*= nullptr*/) const
{
	ComPtr<ID3D::Buffer> _buffer;
	
	HR(mDevice->CreateBuffer(desc, data, _buffer.GetAddressOf()));

	return _buffer;
}

Microsoft::WRL::ComPtr<ID3D::Texture1D> RenderCore::ObjectFactory::CreateTexture1D(const D3D11_TEXTURE1D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data /*= nullptr*/, const char name[] /*= nullptr*/) const
{
	ComPtr<ID3D::Texture1D> _texture;

	HR(mDevice->CreateTexture1D(desc, data, _texture.GetAddressOf()));

	return _texture;
}

Microsoft::WRL::ComPtr<ID3D::Texture2D> RenderCore::ObjectFactory::CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data /*= nullptr*/, const char name[] /*= nullptr*/) const
{
	ComPtr<ID3D::Texture2D> _texture;

	HR(mDevice->CreateTexture2D(desc, data, _texture.GetAddressOf()));

	return _texture;
}

Microsoft::WRL::ComPtr<ID3D::Texture3D> RenderCore::ObjectFactory::CreateTexture3D(const D3D11_TEXTURE3D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data /*= nullptr*/, const char name[] /*= nullptr*/) const
{
	ComPtr<ID3D::Texture3D> _texture;

	HR(mDevice->CreateTexture3D(desc, data, _texture.GetAddressOf()));

	return _texture;
}

Microsoft::WRL::ComPtr<ID3D::RenderTargetView> RenderCore::ObjectFactory::CreateRenderTargetView(ID3D::Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC* desc /*= nullptr*/) const
{
	ComPtr<ID3D::RenderTargetView> _rtv;

	HR(mDevice->CreateRenderTargetView(resource, desc, _rtv.GetAddressOf()));

	return _rtv;
}

Microsoft::WRL::ComPtr<ID3D::ShaderResourceView> RenderCore::ObjectFactory::CreateShaderResourceView(ID3D::Resource* resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc /*= nullptr*/) const
{
	ComPtr<ID3D::ShaderResourceView> _srv;

	HR(mDevice->CreateShaderResourceView(resource, desc, _srv.GetAddressOf()));

	return _srv;
}

Microsoft::WRL::ComPtr<ID3D::UnorderedAccessView> RenderCore::ObjectFactory::CreateUnorderedAccessView(ID3D::Resource* resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* desc /*= nullptr*/) const
{
	ComPtr<ID3D::UnorderedAccessView> _uav;

	HR(mDevice->CreateUnorderedAccessView(resource, desc, _uav.GetAddressOf()));

	return _uav;
}

Microsoft::WRL::ComPtr<ID3D::DepthStencilView> RenderCore::ObjectFactory::CreateDepthStencilView(ID3D::Resource* resource, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc /*= nullptr*/) const
{
	ComPtr<ID3D::DepthStencilView> _dsv;

	HR(mDevice->CreateDepthStencilView(resource, desc, _dsv.GetAddressOf()));

	return _dsv;
}

Microsoft::WRL::ComPtr<ID3D::VertexShader> RenderCore::ObjectFactory::CreateVertexShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::VertexShader> _vs;

	HR(mDevice->CreateVertexShader(bufferPointer, size, linkage, _vs.GetAddressOf()));

	return _vs;
}

Microsoft::WRL::ComPtr<ID3D::PixelShader> RenderCore::ObjectFactory::CreatePixelShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::PixelShader> _ps;

	HR(mDevice->CreatePixelShader(bufferPointer, size, linkage, _ps.GetAddressOf()));

	return _ps;
}

Microsoft::WRL::ComPtr<ID3D::GeometryShader> RenderCore::ObjectFactory::CreateGeometryShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::GeometryShader> _gs;

	HR(mDevice->CreateGeometryShader(bufferPointer, size, linkage, _gs.GetAddressOf()));

	return _gs;
}

Microsoft::WRL::ComPtr<ID3D::HullShader> RenderCore::ObjectFactory::CreateHullShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::HullShader> _hs;

	HR(mDevice->CreateHullShader(bufferPointer, size, linkage, _hs.GetAddressOf()));

	return _hs;
}

Microsoft::WRL::ComPtr<ID3D::DomainShader> RenderCore::ObjectFactory::CreateDomainShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::DomainShader> _ds;

	HR(mDevice->CreateDomainShader(bufferPointer, size, linkage, _ds.GetAddressOf()));

	return _ds;
}

Microsoft::WRL::ComPtr<ID3D::ComputeShader> RenderCore::ObjectFactory::CreateComputeShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage /*= nullptr*/) const
{
	ComPtr<ID3D::ComputeShader> _cs;

	HR(mDevice->CreateComputeShader(bufferPointer, size, linkage, _cs.GetAddressOf()));

	return _cs;
}

Microsoft::WRL::ComPtr<ID3D::ClassLinkage> RenderCore::ObjectFactory::CreateClassLinkage() const
{
	ComPtr<ID3D::ClassLinkage> _linkage;

	mDevice->CreateClassLinkage(_linkage.GetAddressOf());

	return _linkage;
}

Microsoft::WRL::ComPtr<ID3D::DeviceContext> RenderCore::ObjectFactory::CreateDeferredContext() const
{
	ComPtr<ID3D::DeviceContext> _deferredContext;

	HR(mDevice->CreateDeferredContext(0, _deferredContext.GetAddressOf()));

	return _deferredContext;
}

Microsoft::WRL::ComPtr<ID3D::InputLayout> RenderCore::ObjectFactory::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC inputElements[], uint32 inputElementsCount, const void* signature, size_t length) const
{
	ComPtr<ID3D::InputLayout> _inputLayout;

	HR(mDevice->CreateInputLayout(inputElements, inputElementsCount, signature, length, _inputLayout.GetAddressOf()));

	return _inputLayout;
}

Microsoft::WRL::ComPtr<ID3D::Query> RenderCore::ObjectFactory::CreateQuery(const D3D11_QUERY_DESC* desc) const
{
	ComPtr<ID3D::Query> _query;

	HR(mDevice->CreateQuery(desc, _query.GetAddressOf()));

	return _query;
}

void RenderCore::ObjectFactory::InitFactory(ID3D::Device* device)
{
	assert(sObjectFactory == nullptr);

	new ObjectFactory(device);
}

RenderCore::ObjectFactory* RenderCore::ObjectFactory::Instance()
{
	return sObjectFactory;
}

void RenderCore::ObjectFactory::Release()
{
	assert(sObjectFactory != nullptr);

	delete sObjectFactory;

	sObjectFactory = nullptr;
}

