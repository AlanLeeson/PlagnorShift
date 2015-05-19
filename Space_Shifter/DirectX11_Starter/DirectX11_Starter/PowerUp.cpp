#include "PowerUp.h"


PowerUp::PowerUp(Mesh * mesh, Material * material, string type) : Obstacle(mesh,material)
{
	if (type == "rocket"){
		this->setScale(0.2f, 0.2f, 0.2f);
	}
	else if (type == "stopWatch"){
		this->setScale(0.5f, 0.5f, 0.5f);
	}
	this->boundingBox = new BoundingBox(type, this->getPosition(), XMFLOAT3(1, 1, 1));
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

