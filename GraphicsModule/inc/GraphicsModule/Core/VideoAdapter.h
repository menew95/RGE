#pragma once

#include "Common.h"

namespace Graphics
{
	// Format DXGI_FORMAT_R8G8B8A8_UNORM에서의 비디오 어뎁터가 제공하는 해상도와 갱신 빈도
	struct DisplayModeDesc
	{
		Extent2D _resolution;

		uint32 _refreshRate = 0;
	};

	struct VideoOutputDesc
	{
		std::vector<DisplayModeDesc> _displayModes;
	};

	// 비디오 어뎁터 이름, PCI ID, 비디오 메모리의 바이트 수
	struct VideoAdapterDesc
	{
		tstring							_name;
		uint32							_vendorID;
		uint64							_videoMemory = 0;
		std::vector<VideoOutputDesc>	_outputs;
	};
}