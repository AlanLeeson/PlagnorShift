
cbuffer highlight : register(b0)
{
	float threshold;
}

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// TEXTURE STUFF
Texture2D sceneTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Sample all pixels
	float4 finalColor = sceneTexture.Sample(basicSampler, input.uv);

	//Get the highest color channel
	float maxColor = finalColor.x;
	if (maxColor < finalColor.y) maxColor = finalColor.y;
	if (maxColor < finalColor.z) maxColor = finalColor.z;

	//If the max color is too low, set it to black
	if (maxColor < threshold)
	{
		finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return finalColor;
}