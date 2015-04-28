#define blurSize 7


cbuffer blurData : register(b0)
{
	float2 pixelSize;
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
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i <= blurSize * 2; i++)
	{
		float2 offset = float2(0.0f, pixelSize.y * (i - blurSize * 2));
			finalColor += diffuseTexture.Sample(basicSampler, input.uv + offset);
	}
	finalColor /= blurSize * 2;

	//return float4(1.0, 0.0f, 0.0f, 1.0f);
	return finalColor;
}