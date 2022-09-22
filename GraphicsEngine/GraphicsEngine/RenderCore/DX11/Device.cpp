#include "../../pch.h"
#include "IncludeDX11.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"

RenderCore::Device::Device()
{
	mImmediateContext = std::make_shared<DeviceContext>();
	mPresentChain = std::make_shared<SwapChain>();
}

RenderCore::Device::~Device()
{
	
}

void* RenderCore::Device::QueryInterface(const GUID& guid)
{
	return nullptr;
}

void RenderCore::Device::Present()
{
	mPresentChain->Present();
}

std::shared_ptr<RenderCore::IDevice> RenderCore::Device::CreateDevice()
{
	shared_ptr<RenderCore::IDevice> _device = shared_from_this();

	HRESULT _hResult = CoInitialize(NULL);

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#if defined(DEBUG) | defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		mDevice.ReleaseAndGetAddressOf(),
		&mFeatureLevel,
		mImmediateContext->GetDeviceContextRef()
	);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		//return S_FALSE;
	}

	if (mFeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		//return S_FALSE;
	}

	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	return _device;
}
