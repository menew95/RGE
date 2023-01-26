#pragma once

#include "Math/Math.h"

namespace Graphics
{
	struct alignas(16) Camera
	{
		Math::Vector3 _camWorld;
		float _pad;
		Math::Matrix _view;
		Math::Matrix _proj;
		Math::Matrix _projInv;
		Math::Matrix _viewToTexSpace;
		Math::Matrix _worldViewProj;
	};

	struct alignas(16) SSAO
	{
		Math::Vector4 offSetVectors[14];
		Math::Vector4 frustumCorners[4];

		float occlusionRadius;
		float occlusionFadeStart;
		float occlusionFadeEnd;
		float surfaceEpsilon;
	};

	struct alignas(16) PerFrame
	{
		Camera _camera;

		//SSAO _ssao;

	};

	struct alignas(16) PerLight
	{
		uint32 _type;
		float _range;
		float _fallOff;
		float _pad;

		Math::Vector3 _lightPosition;
		float _spotAngle;

		Math::Vector3 _direction;
		float _fallOffAngle;

		Math::Vector3 _color;
		float _intensity;

		Math::Matrix _shadowTransform;
	};

	struct alignas(16) PerLightFrame
	{
		uint32 _lightCount;
		float _iblFactor;
		float _pad[2];
		
		PerLight _perLights[20];
	};
}