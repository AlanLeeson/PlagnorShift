#include "Obstacle.h"

Obstacle::Obstacle(Mesh* mesh, Material* material)
	:GameEntity(mesh, material)
{
	positions = { -3.5f, 0.0f, 3.5f };

	animateMovement = false;
	speed = 10.0f;

	startPosZ = 20.0f;
	finishPosZ = 0.0f;

	this->setPosition(0.0f, -0.75f, startPosZ);
}

Obstacle::~Obstacle(void)
{
}

void Obstacle::ResetLocation(void)
{
	float startX = positions[rand() % 3];
	GameEntity::setPosition(startX, -0.75f, startPosZ);
}

void Obstacle::Update(float dt)
{
	GameEntity::move(0.0f, 0.0f, -speed * dt);
	XMFLOAT3 position = GameEntity::getPosition();

	if (position.z <= finishPosZ)
	{
		ResetLocation();
	}
}