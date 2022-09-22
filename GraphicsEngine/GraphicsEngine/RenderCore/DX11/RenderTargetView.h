#pragma once

#include "IResource.h"

namespace RenderCore
{
	class RenderTargetView
	{
	public:
		RenderTargetView();
		~RenderTargetView();

		ID3D::RenderTargetView* GetView() const { return mRenderTargetView.Get(); }
		ID3D::RenderTargetView* const * GetView() { return mRenderTargetView.GetAddressOf(); }

		//void OnResize(uint32 width = 0, uint32 height = 0, DXGI_FORMAT format = 0);
	
	private:
		ComPtr<ID3D::RenderTargetView> mRenderTargetView;
	};
}