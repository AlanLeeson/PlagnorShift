#pragma once
#include "DirectXGame.h"
#include "Obstacle.h"
#include "PowerUp.h"
#include <vector>

class ObstacleManager
{
public:
	ObstacleManager(int numObstacles, Mesh* mesh, Material* material);
	~ObstacleManager(void);

	int GetCount(void);
	vector<Obstacle*> GetObstacles(void);
	void pushPowerUp(Mesh * mesh, Material* material, string type);
	void Update(float dt);
	void SlowMotion();

private:
	vector<Obstacle*> obstacles;

	float spawnCooldown;
	float timeSinceLastSpawn;

	// Deals with the amount of time that passes since the speed
	// at which the obstacles spawn at has changed
	float spawnFastAfterNumSeconds;
	float timeSinceLastSpawnChange;

	float minSpawnTime;
};