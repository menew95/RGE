#if !defined(H_UTIL)
#define H_UTIL

// 인덱스를 넣으면 인덱스에 따라 큐브 위치값을 xyz에 +1 해서 반환
inline float3 CreateCube(in uint vertexID)
{
	uint b = 1u << vertexID;
	return float3((0x287a & b) != 0, (0x02af & b) != 0, (0x31e3 & b) != 0);
}

inline float3 CreateCubeLine(in uint vertexID)
{
	uint b = 1u << vertexID;
	return float3((0x78c6 & b) != 0, (0x2bf0 & b) != 0, (0x619c & b) != 0);
}
#endif