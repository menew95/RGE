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

	struct alignas(16)  CascadedLight
	{
		Math::Matrix _lightTransform[4];
		Math::Vector4 _cascadeEndClipSpace[4];
	};

	struct alignas(16) Lighting
	{
		uint32 _lightCount;
		Math::Vector3 _pad;

		// cascaded shadow map Info
		CascadedLight _cascadedLightInfo;

		PerLight _perLights[20];
	};

	struct alignas(16) PerPreFiltered
	{
		float _roughness;
	};

	struct alignas(16) PerCubeMapMatrix
	{
		Math::Matrix _view[6];
		Math::Matrix _proj;
	};
}