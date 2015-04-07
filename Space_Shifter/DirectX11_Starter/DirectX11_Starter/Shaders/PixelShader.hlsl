
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer LightData : register(b0)
{
	DirectionalLight light_01;
	DirectionalLight light_02;
	float4 light_am;
}

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float3 light_dir = normalize(-light_01.Direction);
	float light_amount = saturate(dot(input.normal, light_dir));
	float4 light_01_final = light_01.AmbientColor + light_amount * light_01.DiffuseColor;

	light_dir = normalize(-light_02.Direction);
	light_amount = saturate(dot(input.normal, light_dir));
	float4 light_02_final = light_02.AmbientColor + light_amount * light_02.DiffuseColor;

	return (light_01_final + light_02_final + light_am) * surfaceColor;
}