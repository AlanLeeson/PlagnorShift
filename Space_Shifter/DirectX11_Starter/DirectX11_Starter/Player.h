#pragma once
#include "DirectXGame.h"
#include "GameEntity.h"
#include <vector>

class Player :
	public GameEntity
{
public:
	enum Rail { Left, Middle, Right };

	Player(Mesh*, Material*);
	~Player();

	// -1 moves to the left, 1 move to the right
	void move(int direction);

	void Update(float dt);

private:
	vector<float> railPosX;

	Rail currentRail;
	Rail targetRail;

	bool animateMovement;

	float horizontalSpeed;
};

