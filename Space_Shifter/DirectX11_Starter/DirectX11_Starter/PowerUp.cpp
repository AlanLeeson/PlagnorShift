#include "PowerUp.h"


PowerUp::PowerUp(Mesh * mesh, Material * material) : Obstacle(mesh,material)
{
	this->setScale(0.12f, 0.12f, 0.12f);
	this->boundingBox = new BoundingBox("powerUp", this->getPosition(), XMFLOAT3(1, 1, 1));
}


PowerUp::~PowerUp(void)
{
}

void PowerUp::Update(float dt){
	Obstacle::Update(dt);
} 

void PowerUp::fireRocket(float dt)
{
	GameEntity::move(0.0f, 0.0f, 20.0 * dt);
	if (this->getPosition().z > 25)
	{
		this->SetActive(false);
		this->setPosition(0.0f, 0.0f, -10.0f);
	}
}

