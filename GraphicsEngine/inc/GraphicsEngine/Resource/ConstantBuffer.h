#pragma once

#include "Math/Math.h"

#define MAX_LIGHT_COUNT 10

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

	struct CascadedLight
	{
		Math::Vector3 _direction;
		float _pad;
		Math::Vector3 _color;
		float _power;

		Math::Matrix _lightTransform[4];
		Math::Vector4 _cascadeEndClipSpace[4];
	};

	struct alignas(16) DirectionLight
	{
		Math::Vector3 _direction;
		float _pad;

		Math::Vector3 _color;
		float _power;

		Math::Matrix _lightTransform;
	};

	struct alignas(16) PointLight
	{
		Math::Vector3 _position;

		float _range;

		Math::Vector3 _color;

		float _fallOff;

		float _power;
		Math::Vector3 _pad;

		Math::Matrix _lightTransform[6];
	};

	struct alignas(16) SpotLight
	{
		float _range;
		float _fallOff;
		float _pad[2];

		Math::Vector3 _position;
		float _spotAngle;

		Math::Vector3 _direction;
		float _fallOffAngle;

		Math::Vector3 _color;
		float _power;

		Math::Matrix _lightTransform;
	};

	struct alignas(16) PerLight
	{
		// 0 : spot 1 : dir 2 : point
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

	struct alignas(16) Lighting
	{
		// x dir y point z spot
		uint32 _dirLightCount;
		uint32 _pointLightCount;
		uint32 _spotLightCount;
		uint32 _pad;

		CascadedLight _cascadedLight;

		DirectionLight _directionLight[MAX_LIGHT_COUNT];

		SpotLight _spotLight[MAX_LIGHT_COUNT];

		PointLight _pointLight[MAX_LIGHT_COUNT];
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