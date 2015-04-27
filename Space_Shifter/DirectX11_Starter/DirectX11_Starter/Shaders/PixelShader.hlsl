
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 worldPos		: TEXCOORD1;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct Light
{
	float4 AmbientColor;
	float4 DiffuseColor;
};

struct DirectionalLight : Light
{
	float3 Direction;
};

struct PointLight : Light
{
	float3 Position;
	float Distance;
};

cbuffer LightData : register(b0)
{
	DirectionalLight dlight_01;
	DirectionalLight dlight_02;

	PointLight plight_01;
	PointLight plight_02;

	float4 light_am;
}

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	float4 finalLightColor = light_am;

	//Directional Lights
	float3 light_dir = normalize(-dlight_01.Direction);
	float light_amount = saturate(dot(input.normal, light_dir));
	float4 dlight_01_final = dlight_01.AmbientColor + light_amount * dlight_01.DiffuseColor;
	finalLightColor += dlight_01_final;

	light_dir = normalize(-dlight_02.Direction);
	light_amount = saturate(dot(input.normal, light_dir));
	float4 dlight_02_final = dlight_02.AmbientColor + light_amount * dlight_02.DiffuseColor;
	finalLightColor += dlight_02_final;

	//Point Lights
	light_dir = normalize(plight_01.Position - input.worldPos);
	light_amount = saturate(dot(input.normal, light_dir));
	float4 plight_01_final = plight_01.AmbientColor + light_amount * plight_01.DiffuseColor;
	finalLightColor += plight_01_final;

	light_dir = normalize(plight_02.Position - input.worldPos);
	light_amount = saturate(dot(input.normal, light_dir));
	float4 plight_02_final = plight_02.AmbientColor + light_amount * plight_02.DiffuseColor;
	finalLightColor += plight_02_final;

	return finalLightColor * surfaceColor;
}