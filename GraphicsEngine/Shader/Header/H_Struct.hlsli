struct Camera
{
	matrix _camWorld;
	matrix _view;
	matrix _proj;
	matrix _projInv;
	matrix _viewToTexSpace;
	//matrix _viewProj;
	//matrix _worldViewProj;
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
	float4 diffuse;
	float4 emissive;
	float roughness;
	float metallic;
	float2 tiling;
};
