#pragma once

#include "IResource.h"

namespace RenderCore
{
	/// <summary>
	/// 바인딩 될 쉐이더 리소스
	/// </summary>
	/// 
	class ShaderResourceView
	{
	public:
		ShaderResourceView();
		~ShaderResourceView();


		ID3D::ShaderResourceView* GetResource() { return mSRV.Get(); }
		ID3D::ShaderResourceView** GetResourceRef() { return mSRV.GetAddressOf(); }

	private:
		ComPtr<ID3D::ShaderResourceView> mSRV;
	};
}