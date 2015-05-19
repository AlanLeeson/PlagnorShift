#define blurSize 10


cbuffer blurData : register(b0)
{
	int blurAmount;
	float2 pixelSize;
	float moving;
	float direction;
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
Texture2D maskTexture : register(t1);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Sample all pixels
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	float4 maskColor = maskTexture.Sample(basicSampler, input.uv);

	//Blur the non-masked colors
	if (moving == 1.0f && maskColor.x == finalColor.x && maskColor.y == finalColor.y && maskColor.z == finalColor.z)
	{
		for (int i = 0; i <= blurSize; i++)
		{
			float2 offset = float2(pixelSize.x * i, 0.0f);
			offset.x *= direction;
			finalColor += diffuseTexture.Sample(basicSampler, input.uv + offset);
		}
		finalColor /= blurSize;
	}
	else
	{
		finalColor = diffuseTexture.Sample(basicSampler, input.uv);
	}

	//return maskColor;
	return finalColor;
}