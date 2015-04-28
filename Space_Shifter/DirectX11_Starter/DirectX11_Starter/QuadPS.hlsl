
cbuffer whatever : register(b0)
{
	float2 pixelSize;
	int blurAmount;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// TEXTURE STUFF
Texture2D diffuseTexture : register(t0);
Texture2D sceneTexture : register(t1);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Sample all pixels
	float4 blurredColor = diffuseTexture.Sample(basicSampler, input.uv);
	float4 sceneColor = sceneTexture.Sample(basicSampler, input.uv);

	float4 finalColor = sceneColor + blurredColor;

	return finalColor;
}