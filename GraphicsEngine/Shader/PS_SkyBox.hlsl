SamplerState samWrapLinear : register(s0);

TextureCube g_SkyBox : register(t0);

struct VSOutput
{
	float4 posH		: SV_POSITION;
	float3 worldPos	: POSITION;
};

float4 main(VSOutput input) : SV_Target
{
	return g_SkyBox.Sample(samWrapLinear, input.worldPos.xyz);
}