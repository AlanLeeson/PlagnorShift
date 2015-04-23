#pragma once

#include <DirectXMath.h>
#include "Light.h"
#include "SimpleShader.h"

using namespace DirectX;

struct TransformBuffer
{
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;
};

struct LightingBuffer
{
	DirectionalLight d0;
	DirectionalLight d1;
	PointLight p0;
	PointLight p1;
	PointLight p2;
	PointLight p3;
};

class Material
{
public:
	Material(SimpleVertexShader*, SimplePixelShader*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	~Material();

	SimpleVertexShader* getVertexShader(void);
	SimplePixelShader* getPixelShader(void);

	void mapShaderData(TransformBuffer, LightingBuffer);

private:
	SimpleVertexShader* vShader;
	SimplePixelShader* pShader;

	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;
};

