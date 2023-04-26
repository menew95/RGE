struct VoxelRadiance
{
	float3	_gridCenter;
	float	_dataSize;		// ÇÑ º¹¼¿ÀÇ half-extent 
	float	_dataSizeRCP;	// 1.0 / datasize
	uint	_dataRes;		// º¹¼¿ ÇØ»óµµ
	float	_dataResRCP;	// 1.0 / resolution
	uint	_numCones;
	float	_numConesRCP;
	float	_maxDIstance;
	float	_rayStepSize;
	uint	_mips;
};

struct VoxelType
{
	uint _colorMask;
	uint _normalMask;
};

static const float g_hdr_range = 10.f;
static const float g_max_voxel_light = 20;

cbuffer VoxelCB :register(b4)
{
	VoxelRadiance voxel_radiance;
}

// Encode HDR color to a 32 bit uint
// Alpha i 1 bit + 7 bit HDR remapping
inline uint EncodeColor(in float4 color)
{
	// normalize color to LDR
	float hdr = length(color.rgb);
	color.rgb /= hdr;

	// encode LDR color and HDR range
	uint3 _iColor = uint3(color.rgb * 255.0f);
	uint _iHDR = (uint) (saturate(hdr / g_hdr_range) * 127);
	uint _colorMask = (_iHDR << 24u) | (_iColor.r << 16u) | (_iColor.g << 8u) | _iColor.b;

	// encode alpha into highest bit
	uint _iAlpha = (color.a > 0 ? 1u : 0u);
	_colorMask |= _iAlpha << 31u;

	return _colorMask;
}

// Decode 32bit uint into HDR color with 1 bit alpha
inline float4 DecodeColor(in uint colorMask)
{
	float _hdr;
	float4 _color;

	_hdr = (colorMask >> 24u) & 0x0000007f;
	_color.r = (colorMask >> 16u) & 0x000000ff;
	_color.g = (colorMask >> 8u) & 0x000000ff;
	_color.b = colorMask & 0x000000ff;

	_hdr /= 127.0f;
	_color.rgb /= 255.0f;

	_color.rgb *= _hdr * g_hdr_range;
	_color.a = (colorMask >> 31u) * 0x00000001;

	return _color;
}

// Encode specified normal (normalized) into an unsigned integer.
// Each axis of the normal is encoded into 9 bits (1 for the sign/ 8 for the value).
inline uint EncodeNormal(in float3 normal)
{
	int3 _iNormal = int3(normal * 255.0f);
	uint3  _iNormalSigns;

	_iNormalSigns.x = (_iNormal.x >> 5) & 0x04000000;
	_iNormalSigns.y = (_iNormal.y >> 14) & 0x00020000;
	_iNormalSigns.z = (_iNormal.z >> 23) & 0x00000100;
	_iNormal = abs(_iNormal);
	uint _normalMask = _iNormalSigns.x | (_iNormal.x << 18) | _iNormalSigns.y | (_iNormal.y << 9) | _iNormalSigns.z | _iNormal.z;
	return _normalMask;
}

// Docode specified mask into a float3 normal(normalized)
inline float3 DecodeNormal(in uint normalMask)
{
	int3 _iNormal;
	_iNormal.x = (normalMask >> 18) & 0x000000ff;
	_iNormal.y = (normalMask >> 9) & 0x000000ff;
	_iNormal.z = normalMask & 0x000000ff;

	int3 _iNormalSigns;
	_iNormalSigns.x = (normalMask >> 25) & 0x00000002;
	_iNormalSigns.y = (normalMask >> 16) & 0x00000002;
	_iNormalSigns.z = (normalMask >> 7) & 0x00000002;
	_iNormalSigns = 1 - _iNormalSigns;

	float3 _normal = float3(_iNormal) / 255.0f;
	_normal *= _iNormalSigns;
	return _normal;
}

// 3D array index to flattened 1D array index
inline uint Flatten3D(uint3 coord, uint3 dim)
{
	return (coord.z * dim.x * dim.y) + (coord.y * dim.x) + coord.x;
}

// flattened array index to 3D array index
inline uint3 Unflatten3D(uint idx, uint3 dim)
{
	const uint z = idx / (dim.x * dim.y);
	idx -= (z * dim.x * dim.y);
	const uint y = idx / dim.x;
	const uint x = idx % dim.x;
	return uint3(x, y, z);
}