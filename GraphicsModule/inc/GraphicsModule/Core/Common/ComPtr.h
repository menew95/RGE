#pragma once

#include <wrl.h>

namespace Graphics
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}