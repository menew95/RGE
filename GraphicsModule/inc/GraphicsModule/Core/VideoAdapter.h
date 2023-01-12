#pragma once

#include "Common.h"

namespace Graphics
{
	// Format DXGI_FORMAT_R8G8B8A8_UNORM������ ���� ��Ͱ� �����ϴ� �ػ󵵿� ���� ��
	struct DisplayModeDesc
	{
		Extent2D _resolution;

		uint32 _refreshRate = 0;
	};

	struct VideoOutputDesc
	{
		std::vector<DisplayModeDesc> _displayModes;
	};

	// ���� ��� �̸�, PCI ID, ���� �޸��� ����Ʈ ��
	struct VideoAdapterDesc
	{
		tstring							_name;
		uint32							_vendorID;
		uint64							_videoMemory = 0;
		std::vector<VideoOutputDesc>	_outputs;
	};
}