#include "Material.h"


Material::Material(SimpleVertexShader* v, SimplePixelShader* p, ID3D11ShaderResourceView* srv, ID3D11SamplerState* s)
{
	vShader = v;
	pShader = p;
	this->srv = srv;
	sampler = s;
}


Material::~Material()
{
}

SimpleVertexShader* Material::getVertexShader(void)
{
	return vShader;
}

SimplePixelShader* Material::getPixelShader(void)
{
	return pShader;
}

void Material::mapShaderData(TransformBuffer b_transform, LightingBuffer b_lighting)
{
	vShader->SetMatrix4x4("world", b_transform.World);
	vShader->SetMatrix4x4("view", b_transform.View);
	vShader->SetMatrix4x4("projection", b_transform.Projection);

	pShader->SetFloat4("light_am", XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	pShader->SetData(
		"dlight_01",
		&b_lighting.d0,
		sizeof(DirectionalLight));

	pShader->SetData(
		"dlight_02",
		&b_lighting.d1,
		sizeof(DirectionalLight));

	pShader->SetShaderResourceView("diffuseTexture", srv);
	pShader->SetSamplerState("basicSampler", sampler);
}
