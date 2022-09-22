#pragma once

#include "../ID3D.h"
#include <memory>

namespace RenderCore
{
	class Device;

	class ObjectFactory
	{
	public:
		ObjectFactory(Device* device);
		ObjectFactory(ID3D::Device* device);
		~ObjectFactory();

		/// <summary>
		/// Create Objects
		/// </summary>
		
		/// <summary>
		/// State
		/// </summary>
		ComPtr<ID3D::BlendState>		CreateBlendState(const D3D11_BLEND_DESC*) const;
		ComPtr<ID3D::DepthStencilState>	CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC*) const;
		ComPtr<ID3D::RasterizerState>	CreateRasterizerState(const D3D11_RASTERIZER_DESC*) const;
		ComPtr<ID3D::SamplerState>		CreateSamplerState(const D3D11_SAMPLER_DESC*) const;

		/// <summary>
		/// Resource
		/// </summary>
		ComPtr<ID3D::Buffer>	CreateBuffer(const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* data = nullptr, const char name[] = nullptr) const;
		ComPtr<ID3D::Texture1D>	CreateTexture1D(const D3D11_TEXTURE1D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data = nullptr, const char name[] = nullptr) const;
		ComPtr<ID3D::Texture2D>	CreateTexture2D(const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data = nullptr, const char name[] = nullptr) const;
		ComPtr<ID3D::Texture3D>	CreateTexture3D(const D3D11_TEXTURE3D_DESC* desc, const D3D11_SUBRESOURCE_DATA* data = nullptr, const char name[] = nullptr) const;
			
		/// <summary>
		/// View
		/// </summary>
		ComPtr<ID3D::RenderTargetView>		CreateRenderTargetView(ID3D::Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr) const;
		ComPtr<ID3D::ShaderResourceView>	CreateShaderResourceView(ID3D::Resource* resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr) const;
		ComPtr<ID3D::UnorderedAccessView>	CreateUnorderedAccessView(ID3D::Resource* resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* desc = nullptr) const;
		ComPtr<ID3D::DepthStencilView>		CreateDepthStencilView(ID3D::Resource* resource, const D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr) const;
		
		/// <summary>
		/// Shader
		/// </summary>
		ComPtr<ID3D::VertexShader>		CreateVertexShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;
		ComPtr<ID3D::PixelShader>		CreatePixelShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;
		ComPtr<ID3D::GeometryShader>	CreateGeometryShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;
		ComPtr<ID3D::HullShader>		CreateHullShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;
		ComPtr<ID3D::DomainShader>		CreateDomainShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;
		ComPtr<ID3D::ComputeShader>		CreateComputeShader(const void* bufferPointer, size_t size, ID3D::ClassLinkage* linkage = nullptr) const;

		/// <summary>
		/// Shader Dynamic Linking
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D::ClassLinkage>		CreateClassLinkage() const;

		/// <summary>
		/// ETC
		/// </summary>
		//ComPtr<ID3D::DeviceContext>	CreateContext() const;  // ????????????????
		ComPtr<ID3D::DeviceContext>	CreateDeferredContext() const;
		ComPtr<ID3D::InputLayout>	CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC inputElements[], uint32 inputElementsCount, const void* signature, size_t length) const;
		ComPtr<ID3D::Query>			CreateQuery(const D3D11_QUERY_DESC*) const;

		static void InitFactory(ID3D::Device* device);
		static ObjectFactory* Instance();
		static void Release();


		ID3D::Device* GetDevice() { return mDevice.Get(); }

	private:
		ComPtr<ID3D::Device> mDevice;
	};
}