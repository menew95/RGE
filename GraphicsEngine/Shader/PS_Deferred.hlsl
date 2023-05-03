#include "Header\H_Input.hlsli"
#include "Header\H_ConstBuffer.hlsli"
#include "Header\H_Math.hlsli"

Texture2D gAlbedoMap	: register(t0);

#if defined(_NORMAL_MAP)
Texture2D gBumpMap		: register(t1);
#endif

#if defined(_MRA_MAP)
Texture2D gMRAMap		: register(t2);
#endif

SamplerState samWrapLinear	: register(s0);


PSOut main(VSOutput input)
{
	PSOut _out;

#if !defined(_NORMAL_MAP)
	float3 _normal = input.normal;
#else
	float3 normalMapSample = gBumpMap.Sample(samWrapLinear, input.uv).rgb;
	
	float3 bumpedNormalW = { 0.0f, 0.0f, 0.0f };

	NormalSampleToWorldSpace(bumpedNormalW, normalMapSample, input.normal, input.tangent);

	float3 _normal = bumpedNormalW;
#endif

#if !defined(_MRA_MAP)
	float _metallic = material._metallic;

	float _roughness = material._roughness;

	float _ao = 1.0f;
#else
	float3 _MRA = gMRAMap.Sample(samWrapLinear, input.uv).rgb;
	
	// texture의 color공간이 어디서 만들어지냐에 따라 필요 할 수도 있고 안해도 될 수도 있다. 
	//_MRA = pow(_MRA, 2.2);

	float _metallic = _MRA.r;

	float _roughness = _MRA.g;

	float _ao = _MRA.b;
#endif

#if !defined(_ALBEDO_MAP)
	_out.Albedo = float4(material._albedo.xyz, _metallic);
	//_out.Albedo = float4(0.2f, 0.2f, 0.2f, _metallic);
#else
	_out.Albedo = float4(gAlbedoMap.Sample(samWrapLinear, input.uv).xyz, _metallic);
#endif

	_out.Normal = float4((_normal * 0.5 + 0.5), _roughness);

	_out.Depth = float4(input.posH.z, input.posH.z, input.posH.z, _ao);

	_out.World = float4(input.posW.xyz, _ao);

	_out.Emissive = float4(float3(material._emissive.rgb), input.posV.z);

	return _out;
}