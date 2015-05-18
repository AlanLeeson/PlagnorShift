#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Vertex.h"
#include <vector>

using namespace std;
using namespace DirectX;

class BoundingBox
{
public:
	BoundingBox(string, XMFLOAT3, XMFLOAT3);
	~BoundingBox(void);
	string name;

	XMFLOAT3 position;
	XMFLOAT3 size;
	bool collidable;
	//XMFLOAT3 centroid;
	//XMFLOAT3 max;
	//XMFLOAT3 min;

	//XMFLOAT4X4 modelToWorld;

	// string GetName(void);
/*	__declspec(property(get = GetName)) string name;

	XMFLOAT3 GetCentroid(void);
	__declspec(property(get = GetCentroid)) XMFLOAT3 centroid;

	XMFLOAT3 GetMaximum(void);
	__declspec(property(get = GetMaximum)) XMFLOAT3 maximum;

	XMFLOAT3 GetMinimum(void);
	__declspec(property(get = GetMinimum)) XMFLOAT3 minimum;

	XMFLOAT4X4 GetModelMatrix(void);
	void SetModelMatrix(XMFLOAT4X4 modelMatrix);
	__declspec(property(get = GetModelMatrix, put = SetModelMatrix)) XMFLOAT4X4 modelMatrix;*/

private:
	//void Calculate(std::vector<Vertex> vertices);
};