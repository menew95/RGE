#pragma once

#include "GraphicsModule/Module.h"

namespace Graphics
{
	struct RenderingSetting
	{
		// sslr
		bool _ssrl = true;
		Math::Vector2 _depthBufferSize = { 1280, 720 };
		float _zThickness = 0.1f;
		float _nearPlaneZ = 0.1f;

		float _stride = 1.0f;
		float _maxSteps = 300.f;
		float _maxDistance = 100.f;
		float _strideZCutoff = 1.f;

		float _numMips;
		float _fadeStart;
		float _fadeEnd;

		// tone map
		bool _toneMap = true;
		float _exposure = 0.5f;

		// voxel
		bool	_voxelGI = false;
		bool	_voxelDebug = false;
		bool	_voxelDebugLine = false;
		bool	_voxelSecondBounce = false;
		uint32	_voxelUpdateFrame = 1;
		float	_voxelSize = 0.075f;
		uint32	_voxelNumCones = 6;
		float	_voxelRayStepDistance = 0.5f;
		float	_voxelMaxDistance = 20.0f;
		uint32	_voxelMips = 7;

		float	_aoAlpha = 0.01f;
		//float	_aoFalloff = 725.f;
		float	_aoFalloff = 7250.f;
		float	_inDirectFactor = 1.0f;
		uint32	_mode = 0;
		
		float	_temp = 1;
	};

	struct GraphicsEngineDesc
	{
		RenderSystemDesc _renderSystemDesc;
		
		bool _isFullScreen;

		void* _handle;

		uint32 _width;
		uint32 _height;
	};
}