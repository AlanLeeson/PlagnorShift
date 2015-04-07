#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "DirectXGame.h"
#include "Vertex.h"

using namespace DirectX;
class Mesh
{
public:
	Mesh(Vertex*, int, unsigned int*, int, ID3D11Device*);
	Mesh(const char*, ID3D11Device*);
	~Mesh();

	ID3D11Buffer* getVertexBuffer(void);
	ID3D11Buffer* getIndexBuffer(void);

	int getIndexCount(void);

	void draw(ID3D11Device*, ID3D11DeviceContext*);

private:
	Vertex* vertices;
	unsigned int* indicies;
	int vCount;
	int iCount;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	};

