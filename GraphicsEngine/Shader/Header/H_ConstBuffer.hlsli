#include "Header/H_Struct.hlsli"

#if !defined(H_CONSTANTBUFFER)

#define H_CONSTANTBUFFER

#define BONE_MAX_CNT 128


// Per Frame

cbuffer PerCamera : register(b0)
{
	Camera camera;
}

// Per Draw

cbuffer Transform : register(b1)
{
	matrix world;
	matrix worldInvTranspose;
};

cbuffer Lighting : register(b1)
{
	uint LightCount;
	float IBLFactor;
	float2 pad;

	Light LightInfo[MAX_LIGHT_COUNT];
}

#if defined(_SKIN) && defined(BONECNT)
cbuffer PerSkin: register(b3)
{
	matrix boneTransforms[BONE_MAX_CNT];
};
#endif //_SKIN && BONECNT

cbuffer PerMaterial : register(b2)
{
	Material material;
};

#endif // H_CONSTANTBUFFER