#include "Obstacle.h"

Obstacle::Obstacle(Mesh* mesh, Material* material)
	:GameEntity(mesh, material)
{
	positions = { -3.5f, 0.0f, 3.2f };

	animateMovement = false;
	active = false;

	speed = 15.0f;
	startPosZ = 25.0f;
	finishPosZ = -4.0f;
	this->setScale(2.0f, 2.0f, 2.0f);
	this->setPosition(0.0f, -0.25f, -10.0f);
	XMFLOAT3 size = XMFLOAT3(1, 1, 1); 
	this->boundingBox = new BoundingBox("obstacle", this->getPosition(), size);
}

Obstacle::~Obstacle(void)
{
}

float Obstacle::Active(void)
{
	return active;
}

void Obstacle::SetActive(bool active)
{
	this->active = active;
}

void Obstacle::ResetLocation(void)
{
	float startX = positions[rand() % 3];
	this->boundingBox->collidable = true;
	GameEntity::setPosition(startX, -0.25f, startPosZ);
}

bool Obstacle::OutOfBounds(void)
{
	XMFLOAT3 position = GameEntity::getPosition();
	return position.z <= finishPosZ;
}

void Obstacle::Update(float dt)
{
	if (this->boundingBox->collidable == true) 
	{
		GameEntity::move(0.0f, 0.0f, -speed * dt);
	}
	else
	{
		this->SetActive(false);
		this->setPosition(0.0f, -0.25f, -10.0f);
	} 
}

void Obstacle::fireRocket(float dt) 
{

}