#pragma once

#include "GraphicsModule/Module.h"

namespace Graphics
{
	struct GraphicsEngineDesc
	{
		RenderSystemDesc _renderSystemDesc;
		
		bool _isFullScreen;

		void* _handle;

		uint32 _width;
		uint32 _height;
	};
}