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