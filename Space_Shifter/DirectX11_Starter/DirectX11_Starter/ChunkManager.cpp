#include "ChunkManager.h"


ChunkManager::ChunkManager()
{
	resource_manager = &ResourceManager::getInstance();
}


ChunkManager::~ChunkManager()
{
	//delete[] chunk;
}

void ChunkManager::buildChunk()
{
	resource_manager->getMesh("tile", &mesh);
	resource_manager->getMaterial("metal", &mat);
	float angle = 0.0f;

	//chunk = new GameEntity(mesh, mat);
}

/*
GameEntity* ChunkManager::getChunk()
{
	return chunk;
}
*/
