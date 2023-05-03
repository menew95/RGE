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

#endif // H_CONSTANTBUFFER