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
		Math::Matrix _viewToTextureSpaceMatrix;
		float _near;
		float _far;
		Math::Vector2 _pad2;
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

	struct alignas(16) SSLR
	{
		Math::Vector2 cb_depthBufferSize; // dimensions of the z-buffer
		float cb_zThickness; // thickness to ascribe to each pixel in the depth buffer
		float cb_nearPlaneZ; // the camera's near z plane

		float cb_stride; // Step in horizontal or vertical pixels between samples. This is a float
		// because integer math is slow on GPUs, but should be set to an integer >= 1.
		float cb_maxSteps; // Maximum number of iterations. Higher gives better images but may be slow.
		float cb_maxDistance; // Maximum camera-space distance to trace before returning a miss.
		float cb_strideZCutoff; // More distant pixels are smaller in screen space. This value tells at what point to
		// start relaxing the stride to give higher quality reflections for objects far from
		// the camera.

		float cb_numMips; // the number of mip levels in the convolved color buffer
		float cb_fadeStart; // determines where to start screen edge fading of effect
		float cb_fadeEnd; // determines where to end screen edge fading of effect
		float cb_sslr_padding0;
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