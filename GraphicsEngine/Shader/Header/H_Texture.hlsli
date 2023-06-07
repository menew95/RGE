#if !defined(H_TEXTURE)

#define H_TEXTURE

float4 UnpackNormalMap(float4 textureSample)
{
	float2 _normalXY = textureSample.rg;

	_normalXY = _normalXY * float2(2.0f, 2.0f) - float2(1.0f, 1.0f);

	float _normalZ = sqrt(saturate(1.0f - dot(_normalXY, _normalXY)));

	return float4(_normalXY.xy, _normalZ, 1.0f);
}


float3 DecodeEndPointColor(uint color)
{
	int3 _color = int3(color) >> int3(11, 5, 0);

	_color &= int3(31, 63, 31);

	return float3(_color) / float3(31.0f, 63.0f, 31.0f);
}

bool DecondeEndPointsColor(uint payload, out float3 ep0, out float3 ep1)
{
	uint _color0 = payload & 0xfffffu;
	uint _color1 = payload >> 16u;

	bool _opaque_mode = (BC_VERSION > 1) || (_color0 > color1);

	ep0 = DecodeEndPointColor(_color0);
	ep1 = DecodeEndPointColor(_color1);

	return _opaque_mode;
}
#endif
