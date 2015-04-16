#include "Camera.h"


Camera::Camera(float aspectRatio)
{
	XMMATRIX id = XMMatrixIdentity();
	XMStoreFloat4x4(&view, id);
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(P));
	forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	position = XMFLOAT3(0.0f, 2.0f, -5.0f);
	XMVECTOR r = XMQuaternionIdentity();
	XMStoreFloat4(&rotation, r);
	xrot = 0.0f;
	yrot = 0.0f;
	moveSpeed = 1.0f;
	rotSpeed = 0.001f;

	Update(1.0);
}


Camera::~Camera()
{
}

void Camera::resize(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(P));
}

XMFLOAT4X4 Camera::getViewMatrix(void)
{
	return view;
}

XMFLOAT4X4 Camera::getProjectionMatrix(void)
{
	return projection;
}

void Camera::Update(float dt)
{
	moveCamera(dt);
	//Update view matrix
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR u = XMLoadFloat3(&up);
	XMVECTOR f = XMLoadFloat3(&forward);

	XMVECTOR q = XMQuaternionRotationRollPitchYaw(xrot, yrot, 0.0f);

	XMVECTOR r = XMVector3Rotate(f, q);
	r = XMVector3Normalize(r);
	XMStoreFloat4(&rotation, q);
	XMMATRIX v = XMMatrixLookToLH(pos, r, u);
	XMStoreFloat4x4(&view, XMMatrixTranspose(v));
}

void Camera::moveCamera(float dt)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	XMVECTOR f = XMLoadFloat3(&forward);
	XMVECTOR u = XMLoadFloat3(&up);
	XMVECTOR r = XMVector3Cross(f, u);
	XMVECTOR q = XMLoadFloat4(&rotation);
	XMVECTOR rotF = XMVector3Rotate(f, q);
	XMVECTOR rotU = XMVector3Rotate(u, q);
	XMVECTOR rotR = XMVector3Rotate(r, q);
	//Normalize the rotated vectors
	rotF = XMVector3Normalize(rotF);
	rotU = XMVector3Normalize(rotU);
	rotR = XMVector3Normalize(rotR);
	//Scale the vectors by moveSpeed
	rotF = XMVectorScale(rotF, moveSpeed * dt);
	rotR = XMVectorScale(rotR, moveSpeed * dt);


	if (GetAsyncKeyState('W') & 0x8000)
	{
		pos = XMVectorAdd(pos, rotF);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		pos = XMVectorAdd(pos, -rotF);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		pos = XMVectorAdd(pos, rotR);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		pos = XMVectorAdd(pos, -rotR);
	}
	XMStoreFloat3(&position, pos);
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		position.y += 0.5f * dt;
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		position.y -= 0.5f * dt;
	}
}

void Camera::rotateCameraRoll(float amount)
{
	xrot += -rotSpeed * amount;
}

void Camera::rotateCameraPitch(float amount)
{
	yrot += rotSpeed * amount;
}