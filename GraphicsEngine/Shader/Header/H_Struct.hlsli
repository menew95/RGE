#if !defined(H_STRUCT)

#define H_STRUCT

struct Camera
{
	float3 _camWorld;
	float _pad;
	matrix _view;
	matrix _proj;
	matrix _projInv;
	matrix _viewToTexSpace;
	matrix _worldViewProj;
	//matrix _viewProj;
};

struct SSAOInfo
{
	float4 offSetVectors[14];
	float4 frustumCorners[4];

	float occlusionRadius;
	float occlusionFadeStart;
	float occlusionFadeEnd;
	float surfaceEpsilon;
};

struct Material
{
	float4 _diffuse;
	float4 _emissive;
	float _roughness;
	float _metallic;
	float2 _tiling;
};

#define MAX_LIGHT_COUNT 10

struct Light
{
	uint Type;

	float Range;		// Point, Spot
	float FallOff;		// Point, Spot
	uint pad;

	float3 Position;	// Point, Spot
	float SpotAngle;	// Spot

	float3 Direction;	// Directional, Spot
	float FallOffAngle;	// Spot

	float3 Color;
	float Power;
	matrix ShadowTransform;

};

struct CascadedLight
{
	float3 _direction;
	float _pad1;

	float3 _color;
	float _power;

	matrix _lightTransform[4];

	float _cascadeEndClipSpace[4];
	float3 _pad2;
};

struct DirectionLight
{
	float3 _direction;
	float _pad;

	float3 _color;
	float _power;

	matrix _lightTransform;
};

struct PointLight
{
	float3 _position;
	float _range;

	float3 _color;
	float _fallOff;

	float _power;
	float3 _pad;

	matrix _lightTransform[6];
};

struct SpotLight
{
	float _range;
	float _fallOff;
	float2 _pad;

	float3 _position;
	float _spotAngle;

	float3 _direction;
	float _fallOffAngle;

	float3 _color;
	float _power;

	matrix _lightTransform;
};

struct CascadedInfo
{
	matrix _cascadedLightTM[4];
	float _cascadeEndClipSpace[4];
};
#endif // H_STRUCT