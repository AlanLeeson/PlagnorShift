#pragma once
#include "DirectXGame.h"
#include "GameEntity.h"

class Player :
	public GameEntity
{
public:
	Player(Mesh*, Material*);
	~Player();

private:
	void movePlayer();
};

