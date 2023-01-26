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

#define MAX_LIGHT_COUNT 20

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

#endif // H_STRUCT