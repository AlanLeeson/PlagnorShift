#include "PowerUp.h"


PowerUp::PowerUp(Mesh * mesh, Material * material) : Obstacle(mesh,material)
{
	this->setScale(0.12f, 0.12f, 0.12f);
	this->boundingBox = new BoundingBox("powerUp", this->getPosition(), XMFLOAT3(1, 1, 1));
}


PowerUp::~PowerUp(void)
{
}
