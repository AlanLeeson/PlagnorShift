#pragma once
#include "DirectXGame.h"
#include "Obstacle.h"
#include <vector>

class PowerUp : public Obstacle
{
public:
    PowerUp(Mesh *, Material *, string type );
    ~PowerUp(void);
	virtual void Update(float dt);
	void fireRocket(float dt);
private:
};

