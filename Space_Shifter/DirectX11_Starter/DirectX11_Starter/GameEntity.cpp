#include "GameEntity.h"


GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
	XMMATRIX identity = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, identity);
	XMVECTOR zero = XMVectorZero();
	XMVECTOR quat_ident = XMQuaternionIdentity();
	XMStoreFloat3(&_position, zero);
	XMStoreFloat3(&_rotation, quat_ident);
	XMStoreFloat3(&_scale, zero);
	//Keep scale from being 0 initially
	setScale(1.0f);
	isMatrixDirty = true;
}


GameEntity::~GameEntity()
{
	mesh = nullptr;
}

void GameEntity::setPosition(XMFLOAT3 newPos)
{
	this->_position = newPos;
	isMatrixDirty = true;
}

void GameEntity::setPosition(float x, float y, float z)
{
	this->_position = XMFLOAT3(x, y, z);
	isMatrixDirty = true;
}

void GameEntity::setRotation(XMFLOAT3 newRot)
{
	this->_rotation = newRot;
	isMatrixDirty = true;
}

void GameEntity::setRotation(float x, float y, float z)
{
	this->_rotation = XMFLOAT3(x, y, z);
	isMatrixDirty = true;
}

void GameEntity::setScale(XMFLOAT3 newScale)
{
	this->_scale = newScale;
	isMatrixDirty = true;
}

void GameEntity::setScale(float x, float y, float z)
{
	this->_scale = XMFLOAT3(x, y, z);
	isMatrixDirty = true;
}

void GameEntity::setScale(float size)
{
	this->_scale = XMFLOAT3(size, size, size);
}

void GameEntity::setMaterial(Material* mat)
{
	this->material = mat;
}

XMFLOAT4X4 GameEntity::getWorldMatrix(void)
{
	if (isMatrixDirty)
	{
		createWorldMatrix();
	}
	return this->worldMatrix;
}

XMFLOAT3 GameEntity::getPosition(void)
{
	return this->_position;
}

XMFLOAT3 GameEntity::getRotation(void)
{
	return this->_rotation;
}

XMFLOAT3 GameEntity::getScale(void)
{
	return this->_scale;
}

bool GameEntity::getIsMatrixDirty(void)
{
	return this->isMatrixDirty;
}

Material* GameEntity::getMaterial(void)
{
	return this->material;
}

void GameEntity::move(XMFLOAT3 amount)
{
	XMVECTOR v_pos = XMLoadFloat3(&_position);
	XMVECTOR v_vector = XMLoadFloat3(&amount);
	v_pos = XMVectorAdd(v_pos, v_vector);
	XMStoreFloat3(&_position, v_pos);
	isMatrixDirty = true;
}

void GameEntity::move(float x, float y, float z)
{
	XMFLOAT3 vector = XMFLOAT3(x, y, z);
	move(vector);
}

void GameEntity::moveForward(float amount)
{
	isMatrixDirty = true;
}

void GameEntity::rotate(XMFLOAT3 amount)
{
	XMVECTOR v_rot = XMLoadFloat3(&_rotation);
	XMVECTOR v_amount = XMLoadFloat3(&amount);
	v_rot = XMVectorAdd(v_rot, v_amount);
	XMStoreFloat3(&_rotation, v_rot);
	isMatrixDirty = true;
}

void GameEntity::rotate(float x, float y, float z)
{
	XMFLOAT3 amount = XMFLOAT3(x, y, z);
	rotate(amount);
}

void GameEntity::scale(XMFLOAT3 amount)
{
	XMVECTOR v_scale = XMLoadFloat3(&_scale);
	XMVECTOR v_vector = XMLoadFloat3(&amount);
	v_scale = XMVectorAdd(v_scale, v_vector);
	XMStoreFloat3(&_scale, v_scale);
	isMatrixDirty = true;
	isMatrixDirty = true;
}

void GameEntity::scale(float x, float y, float z)
{
	XMFLOAT3 amount = XMFLOAT3(x, y, z);
	scale(amount);
}

void GameEntity::scale(float size)
{
	XMFLOAT3 amount = XMFLOAT3(size, size, size);
	scale(amount);
}

void GameEntity::createWorldMatrix()
{
	XMMATRIX W = XMMatrixIdentity();

	XMVECTOR v_pos = XMLoadFloat3(&_position);
	XMVECTOR v_rot = XMLoadFloat3(&_rotation);
	XMVECTOR v_quat = XMQuaternionRotationRollPitchYawFromVector(v_rot);
	XMVECTOR v_scale = XMLoadFloat3(&_scale);

	XMMATRIX m_trans = XMMatrixTranslationFromVector(v_pos);
	XMMATRIX m_rot = XMMatrixRotationQuaternion(v_quat);
	XMMATRIX m_scale = XMMatrixScalingFromVector(v_scale);

	W = W * m_scale * m_rot * m_trans;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));
}

void GameEntity::setShaderVariables()
{

}

void GameEntity::draw(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (isMatrixDirty)
	{
		createWorldMatrix();
	}


	SimpleVertexShader* vShader = material->getVertexShader();
	//vShader->SetMatrix4x4("world", worldMatrix);
	//vShader->SetMatrix4x4("view", camera->getViewMatrix());
	//vShader->SetMatrix4x4("projection", camera->getProjectionMatrix());
	vShader->SetShader();

	SimplePixelShader* pShader = material->getPixelShader();
	pShader->SetShader();

	mesh->draw(device, context);

	isMatrixDirty = false;
}
