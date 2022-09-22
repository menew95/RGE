#pragma once

#include "IResource.h"

namespace RenderCore
{
	class VertexShader //: public IResource<VertexShader>
	{
	public:
		VertexShader();
		~VertexShader();

		ID3D::VertexShader* GetShader() const  { return mVertexShader.Get(); }
		ID3D::VertexShader** GetShaderRef() { return mVertexShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const  { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::VertexShader> mVertexShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};

	class PixelShader //: public IResource<PixelShader>
	{
	public:
		PixelShader();
		~PixelShader();

		ID3D::PixelShader* GetShader() const  { return mPixelShader.Get(); }
		ID3D::PixelShader** GetShaderRef() { return mPixelShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::PixelShader> mPixelShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};

	class GeometryShader //: public IResource<GeometryShader>
	{
	public:
		GeometryShader();
		~GeometryShader();

		ID3D::GeometryShader* GetShader() const { return mGeometryShader.Get(); }
		ID3D::GeometryShader** GetShaderRef() { return mGeometryShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::GeometryShader> mGeometryShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};

	class HullShader //: public IResource<HullShader>
	{
	public:
		HullShader();
		~HullShader();

		ID3D::HullShader* GetShader() const { return mHullShader.Get(); }
		ID3D::HullShader** GetShaderRef() { return mHullShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::HullShader> mHullShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};

	class DomainShader //: public IResource<DomainShader>
	{
	public:
		DomainShader();
		~DomainShader();

		ID3D::DomainShader* GetShader() const { return mDomainShader.Get(); }
		ID3D::DomainShader** GetShaderRef() { return mDomainShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::DomainShader> mDomainShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};

	class ComputeShader //: public IResource<ComputeShader>
	{
	public:
		ComputeShader();
		~ComputeShader();

		ID3D::ComputeShader* GetShader() const { return mComputeShader.Get(); }
		ID3D::ComputeShader** GetShaderRef() { return mComputeShader.GetAddressOf(); }
		ID3D::ClassLinkage* GetClassLinkage() const { return mClassLinkage.Get(); }
		ID3D::ClassLinkage** GetClassLinkageRef() { return mClassLinkage.GetAddressOf(); }

	private:
		ComPtr<ID3D::ComputeShader> mComputeShader;
		ComPtr<ID3D::ClassLinkage> mClassLinkage;
	};
}