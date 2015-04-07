#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 UV;
};