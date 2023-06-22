#if !defined(H_INPUT)

#define H_INPUT

#if !defined(BONECNT)
	#define BONECNT 4
#endif

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
    uint	bone[BONECNT]   : BONE;
    float	weight[BONECNT] : WEIGHT;
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

#if defined(_NORMAL_MAP)
	float3 tangent	: TANGENT;
#endif

#if defined(INSTANCING)
	float4 albedoColor : COLOR1;
	float4 emissiveColor : COLOR2;
	float4 pbr : COLOR3;
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

#if defined(HARDWARE)
struct VSInstanceInput
{
	float4x4	_world		: INSTANCE_WORLD;
	float4x4	_worldInv	: INSTANCE_WORLDINV;

	// material property
	float4		_albedo		: INSTANCE_ALBEDO;
	float4		_emissive	: INSTANCE_EMISSIVE;
	float4		_pbr		: INSTANCE_PBR;
};
#endif

#endif // H_INPUT