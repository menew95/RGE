#pragma once

#include "Prefix.h"
#include "Types.h"
#include <wrl.h>

// DX 11 or DX 12
#define DX_VERSION              DX_VERSION_11
#if DX_VERSION == DX_VERSION_11
#include "DX11/ID3D11.h"
#else
#include "DX12/ID3D12.h"
#endif // DX_VERSION == DX_VERSION_11

struct _GUID;
typedef struct _GUID GUID;
typedef long HRESULT;
enum D3D_FEATURE_LEVEL;

struct IDXGIDevice1;
struct IDXGISwapChain;
struct IDXGIAdapter1;
struct IDXGIFactory1;
typedef enum DXGI_FORMAT DXGI_FORMAT;

namespace IDXGI
{
	using Device = IDXGIDevice1;
	using SwapChain = IDXGISwapChain;
	using Adapter = IDXGIAdapter1;
	using Factory = IDXGIFactory1;
}

using namespace Microsoft::WRL;