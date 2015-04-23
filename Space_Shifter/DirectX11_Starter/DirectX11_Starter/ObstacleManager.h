#pragma once
#include "DirectXGame.h"
#include "Obstacle.h"
#include <vector>

class ObstacleManager
{
public:
	ObstacleManager(int numObstacles, Mesh* mesh, Material* material);
	~ObstacleManager(void);

	int GetCount(void);
	vector<Obstacle*> GetObstacles(void);
	void Update(float dt);

private:
	vector<Obstacle*> obstacles;

	float spawnCooldown;
	float timeSinceLastSpawn;
};