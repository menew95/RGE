#include "Header/H_Voxel.hlsli"

struct VSOutput
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

Texture3D<float4> voxel_texture : register(t0);

VSOutput main(uint vertexID : SV_VERTEXID)
{
	VSOutput _output;

	uint3 _coord = Unflatten3D(vertexID, voxel_radiance._dataRes);
	float4 _voxel = voxel_texture[_coord];

	_output.pos = _coord;
	_output.color = _voxel;

	return _output;
}