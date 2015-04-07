#pragma once

#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class Camera
{
public:
	Camera(float);
	~Camera();

	void resize(float);

	XMFLOAT4X4 getViewMatrix(void);
	XMFLOAT4X4 getProjectionMatrix(void);

	void Update(float);
	void moveCamera(float);
	void rotateCameraRoll(float);
	void rotateCameraPitch(float);


private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;

	XMFLOAT3 up;
	XMFLOAT3 forward;

	XMFLOAT3 position;
	XMFLOAT4 rotation;

	float xrot;
	float yrot;

	float moveSpeed;
	float rotSpeed;

};

