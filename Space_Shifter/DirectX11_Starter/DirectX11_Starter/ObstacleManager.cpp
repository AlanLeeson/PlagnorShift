#include "ObstacleManager.h"

ObstacleManager::ObstacleManager(int numObstacles, Mesh* mesh, Material* material)
{
	spawnCooldown = 1.0f;
	timeSinceLastSpawn = 0.0f;

	for (int i = 0; i < numObstacles; i++)
	{
		obstacles.push_back(new Obstacle(mesh, material));
	}
}

ObstacleManager::~ObstacleManager()
{
}

int ObstacleManager::GetCount(void)
{
	return obstacles.size();
}

vector<Obstacle*> ObstacleManager::GetObstacles(void)
{
	return obstacles;
}

void ObstacleManager::Update(float dt)
{
	for (int i = 0; i < obstacles.size(); i++)
	{
		Obstacle* obstacle = obstacles[i];
		obstacle->Update(dt);

		if (obstacle->OutOfBounds())
			obstacle->ResetLocation();
	}
}