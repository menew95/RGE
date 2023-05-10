#include "Header/H_Struct.hlsli"

#if !defined(H_CONSTANTBUFFER)

#define H_CONSTANTBUFFER

#define BONE_MAX_CNT 128


// Per Frame

cbuffer PerCamera : register(b0)
{
	Camera camera;
};

// Per Draw

cbuffer Transform : register(b1)
{
	matrix world;
	matrix worldInvTranspose;
};

#if defined(_SKIN) && defined(BONECNT)
cbuffer PerSkin: register(b2)
{
	matrix boneTransforms[BONE_MAX_CNT];
};
#endif //_SKIN && BONECNT

cbuffer PerMaterial : register(b3)
{
	Material material;
};

cbuffer Lighting : register(b4)
{
	// x dir y point z spot
	uint4 _lightCount;

	CascadedLight _cascadedLight;

	DirectionLight _directionLight[MAX_LIGHT_COUNT];

	SpotLight _spotLight[MAX_LIGHT_COUNT];

	PointLight _pointLight[MAX_LIGHT_COUNT];
};

cbuffer PostProcess : register(b6)
{
	// sslr
	float2 _depthBufferSize; // dimensions of the z-buffer
	float _zThickness; // thickness to ascribe to each pixel in the depth buffer
	float _nearPlaneZ; // the camera's near z plane

	float _stride; // Step in horizontal or vertical pixels between samples. This is a float
	// because integer math is slow on GPUs, but should be set to an integer >= 1.
	float _maxSteps; // Maximum number of iterations. Higher gives better images but may be slow.
	float _maxDistance; // Maximum camera-space distance to trace before returning a miss.
	float _strideZCutoff; // More distant pixels are smaller in screen space. This value tells at what point to
	// start relaxing the stride to give higher quality reflections for objects far from
	// the camera.

	float _numMips; // the number of mip levels in the convolved color buffer
	float _fadeStart; // determines where to start screen edge fading of effect
	float _fadeEnd; // determines where to end screen edge fading of effect

	// tone map
	float g_fHardExposure;


	float4 _pad;  // 64bit
}

#endif // H_CONSTANTBUFFER