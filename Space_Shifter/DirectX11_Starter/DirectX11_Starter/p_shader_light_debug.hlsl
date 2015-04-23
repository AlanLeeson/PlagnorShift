struct VertexToPixel
{
	float4 position		: SV_POSITION;
};

cbuffer LightColor : register( b0 )
{
	float4 color;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	return color;
}