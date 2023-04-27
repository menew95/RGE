struct PSInput
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
	return input.color;
}