#pragma once

#include "IResource.h"

namespace RenderCore
{
	/// <summary>
	/// ���ε� �� ���̴� ���ҽ�
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