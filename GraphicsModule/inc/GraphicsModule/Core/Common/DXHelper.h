#pragma once

#include "Common.h"

namespace Graphics
{
	void DXThrowIfFailed(const HRESULT hr, const char* info);
}