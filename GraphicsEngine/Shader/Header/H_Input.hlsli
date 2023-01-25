#if !defined(H_INPUT)

#define H_INPUT

struct VSInput
{
#if defined(_SCREEN)
	float3	 posL			: POSITION;
	float2	 uv				: TEXCOORD;
#else
	float3	 posL			: POSITION;
	float4	 color			: COLOR;
	float2	 uv				: TEXCOORD;
	float3	 normal			: NORMAL;
//#ifdef _NORMAL_MAP
	float3	tangent			: TANGENT;
//#endif
//#if defined(_SKIN) && defined(BONECNT)
    uint	bone[4]   : BONE;
    float	weight[4] : WEIGHT;
//#endif //_SKIN && BONECNT
#endif
};

struct VSOutput
{
	float4 posH		: SV_POSITION;
	float4 posW		: POSITION1;
	float4 posV		: POSITION2;
	float4 color	: COLOR;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD1;

#ifdef _NORMAL_MAP
	float3 tangent	: TANGENT;
#endif
};

struct PSOut
{
	float4 Albedo	: SV_TARGET0;
	float4 Normal	: SV_TARGET1;
	float4 Depth	: SV_TARGET2;
	float4 World	: SV_TARGET3;
	float4 Emissive	: SV_TARGET4;
};

#endif // H_INPUT