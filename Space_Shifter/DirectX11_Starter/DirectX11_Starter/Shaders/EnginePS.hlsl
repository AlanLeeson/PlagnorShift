
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float2 uv			: UV;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	//float4 finalColor = mul(surfaceColor, input.color);
	float4 finalColor =surfaceColor * input.color;

	return finalColor;
}