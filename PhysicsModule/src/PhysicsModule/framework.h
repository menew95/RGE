#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#pragma warning(disable: 4099)

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "PhysX_Debug_64.lib")
#pragma comment(lib, "PhysXCommon_Debug_64.lib")
#pragma comment(lib, "PhysXFoundation_Debug_64.lib")
#pragma comment(lib, "PhysXCooking_Debug_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#else // Release
#pragma comment(lib, "PhysX_Release_64.lib")
#pragma comment(lib, "PhysXCommon_Release_64.lib")
#pragma comment(lib, "PhysXFoundation_Release_64.lib")
#pragma comment(lib, "PhysXCooking_Release_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#endif

// Windows 헤더 파일
#include <windows.h>
#include <cassert>
#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "PhysicsModule\PhysicsInfo.h"
#include "PhysicsModule\PhysicsHelper.h"

#include "Common.h"
