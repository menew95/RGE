#pragma once

#include "../../RenderCommon/IDevice.h"

namespace RenderCore
{
	class DeviceContext;
	class SwapChain;

	class Device :
		public IDevice, public std::enable_shared_from_this<Device>
	{
	public:
		Device();
		virtual ~Device();

		void* QueryInterface(const GUID& guid) override;
		void Present() override;
		std::shared_ptr<IDevice> CreateDevice() override;

		ID3D::Device* GetDevice() { return mDevice.Get(); }

	private:
		D3D_FEATURE_LEVEL mFeatureLevel;
		uint32 m4xMsaaQuality;

		ComPtr<ID3D::Device> mDevice;

		shared_ptr<DeviceContext> mImmediateContext;
		shared_ptr<SwapChain> mPresentChain;
	};
}

