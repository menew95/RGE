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
		float _exposure = 1.0f;


		// Voxel
		bool	_voxelGI = false;
		bool	_voxelDebug = false;
		bool	_voxelDebugLine = false;
		bool	_voxelSecondBounce = false;
		uint32	_voxelUpdateFrame = 1;
		float	_voxelSize = 0.1f;
		uint32	_voxelNumCones = 2;
		float	_voxelRayStepDistance = 0.75f;
		float	_voxelMaxDistance = 20.0f;
		uint32	_voxelMips = 7;
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