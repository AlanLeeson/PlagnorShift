#pragma once
#include <DirectXMath.h>
#include "DirectXGame.h"
#include "GameEntity.h"
#include "ResourceManager.h"

using namespace DirectX;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void buildChunk(void);
	//GameEntity* getChunk(void);
private:
	ResourceManager* resource_manager;
	//Current location to build next chunk
	XMFLOAT3 loc;

	//GameEntity chunk[7];
	Mesh* mesh;
	Material* mat;
};

