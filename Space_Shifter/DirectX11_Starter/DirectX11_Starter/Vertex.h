#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

//BE CAREFUL OF PACKING
struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 UV;
	XMFLOAT3 Normal;
};

struct PVertex
{
	int Type;
	float Age;
	XMFLOAT3 StartPosition;
	XMFLOAT3 StartVelocity;
	XMFLOAT4 StartColor;
	XMFLOAT4 MidColor;
	XMFLOAT4 EndColor;
	XMFLOAT3 StartMidEndSize;
};