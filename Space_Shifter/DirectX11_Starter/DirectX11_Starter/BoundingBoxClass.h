#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Vertex.h"
#include <vector>

using namespace std;
using namespace DirectX;

class BoundingBoxClass 
{
	string name;

	XMFLOAT3 centroid;
	XMFLOAT3 max;
	XMFLOAT3 min;

	XMFLOAT4X4 modelToWorld;

public:
	BoundingBoxClass(string instanceName);
	BoundingBoxClass(BoundingBoxClass const& other);
	BoundingBoxClass& operator=(BoundingBoxClass const& other);
	~BoundingBoxClass(void);

	string GetName(void);
	__declspec(property(get = GetName)) string name;

	XMFLOAT3 GetCentroid(void);
	__declspec(property(get = GetCentroid)) XMFLOAT3 centroid;

	XMFLOAT3 GetMaximum(void);
	__declspec(property(get = GetMaximum)) XMFLOAT3 maximum;

	XMFLOAT3 GetMinimum(void);
	__declspec(property(get = GetMinimum)) XMFLOAT3 minimum;

	XMFLOAT4X4 GetModelMatrix(void);
	void SetModelMatrix(XMFLOAT4X4 modelMatrix);
	__declspec(property(get = GetModelMatrix, put = SetModelMatrix)) XMFLOAT4X4 modelMatrix;

private:
	void Calculate(std::vector<Vertex> vertices);
};