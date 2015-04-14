
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
	DirectionalLight dlight_01;
	DirectionalLight dlight_02;
	float4 light_am;
}

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float3 light_dir = normalize(-dlight_01.Direction);
	float light_amount = saturate(dot(input.normal, light_dir));
	float4 dlight_01_final = dlight_01.AmbientColor + light_amount * dlight_01.DiffuseColor;

	light_dir = normalize(-dlight_02.Direction);
	light_amount = saturate(dot(input.normal, light_dir));
	float4 dlight_02_final = dlight_02.AmbientColor + light_amount * dlight_02.DiffuseColor;

	return (dlight_01_final + dlight_02_final + light_am) * surfaceColor;
}