
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
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Sample all pixels
	float4 finalColor = diffuseTexture.Sample(basicSampler, input.uv);

		/*
		for (int blur = 0; blur < blurAmount; blur++)
		{
		[unroll]
		for (int i = 1; i < 9; i++)
		{
		finalColor += diffuseTexture.Sample(basicSampler, input.uv + uvOffset[i] * blur);
		}
		}

		finalColor /= max(1, 9 * blurAmount);
		*/

	//return float4(input.uv.x, input.uv.y, 0.0f, 1.0f);
	return finalColor;
}