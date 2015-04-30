#include "PowerUp.h"


PowerUp::PowerUp(Mesh * mesh, Material * material) : Obstacle(mesh,material)
{
	this->setScale(0.12f, 0.12f, 0.12f);
}


PowerUp::~PowerUp(void)
{
}
