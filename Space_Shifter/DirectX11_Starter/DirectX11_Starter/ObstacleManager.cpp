#include "ObstacleManager.h"

ObstacleManager::ObstacleManager(int numObstacles, Mesh* mesh, Material* material)
{
	spawnCooldown = 1.5f;
	timeSinceLastSpawn = 0.0f;

	spawnFastAfterNumSeconds = 5.0f;
	timeSinceLastSpawnChange = 0.0f;

	minSpawnTime = 0.5f;

	for (int i = 0; i < numObstacles; i++)
	{
		obstacles.push_back(new Obstacle(mesh, material));
	}
}

ObstacleManager::~ObstacleManager()
{
	for (int i = 0; i < GetCount(); i++)
	{
		delete obstacles[i];
	}
}

int ObstacleManager::GetCount(void)
{
	return obstacles.size();
}

void ObstacleManager::SlowMotion()
{
	for (int i = 0; i < GetCount(); i++)
	{
		obstacles[i]->SlowMotion();
	}
}

void ObstacleManager::reset()
{
	for (int i = 0; i < GetCount(); i++)
	{
		obstacles[i]->reset();
	}

	spawnCooldown = 1.5f;
	timeSinceLastSpawn = 0.0f;

	spawnFastAfterNumSeconds = 5.0f;
	timeSinceLastSpawnChange = 0.0f;

	minSpawnTime = 0.5f;
}

void ObstacleManager::pushPowerUp(Mesh * mesh, Material * material, string type){
	obstacles.push_back(new PowerUp(mesh, material, type));
}

vector<Obstacle*> ObstacleManager::GetObstacles(void)
{
	return obstacles;
}

void ObstacleManager::Update(float dt)
{
	timeSinceLastSpawn += dt;
	timeSinceLastSpawnChange += dt;

	for (int i = 0; i < GetCount(); i++)
	{
		Obstacle* obstacle = obstacles[i];

		// Check if this obstacle is active
		if (obstacle->Active())
		{
			obstacle->Update(dt);

			// Check if the obstacle if it is out of bounds
			if (obstacle->OutOfBounds())
			{
				obstacle->SetActive(false);
			}
		}
		else
		{
			// Check if we can spawn this obstacle
			if (timeSinceLastSpawn >= spawnCooldown && rand() % GetCount() == 0)
			{
				obstacle->SetActive(true);
				obstacle->ResetLocation();
				timeSinceLastSpawn = 0.0f;
			}
		}
	}

	if (timeSinceLastSpawnChange >= spawnFastAfterNumSeconds)
	{
		if (spawnCooldown > minSpawnTime)
		{
			spawnCooldown *= 0.9f;
			timeSinceLastSpawnChange = 0.0f;

			for (int i = 0; i < GetCount(); i++)
			{
				obstacles[i]->SpeedUp(2.5f);
			}
		}
		else
		{
			spawnCooldown = minSpawnTime;
		}
	}
}