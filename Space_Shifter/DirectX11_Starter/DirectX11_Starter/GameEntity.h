#pragma once
#include "DirectXGame.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "VSConstantBufferLayout.h"

class GameEntity
{
public:
	GameEntity(Mesh*, Material*);
	~GameEntity();

	void setPosition(XMFLOAT3);
	void setPosition(float, float, float);
	void setRotation(XMFLOAT3);
	void setRotation(float, float, float);
	void setScale(XMFLOAT3);
	void setScale(float, float, float);
	void setScale(float);
	void setMaterial(Material*);

	XMFLOAT4X4 getWorldMatrix(void);
	XMFLOAT3 getPosition(void);
	XMFLOAT3 getRotation(void);
	XMFLOAT3 getScale(void);
	bool getIsMatrixDirty(void);
	Material* getMaterial(void);

	//virtual void setShaderVariables();

	void move(XMFLOAT3);
	void move(float, float, float);
	void moveForward(float);
	void rotate(XMFLOAT3);
	void rotate(float, float, float);
	void scale(XMFLOAT3);
	void scale(float, float, float);
	void scale(float);

	void createWorldMatrix(void);
	void draw(ID3D11Device*, ID3D11DeviceContext*);

private:
	Mesh* mesh;
	Material* material;
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 _position;
	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;

	bool isMatrixDirty;
};

