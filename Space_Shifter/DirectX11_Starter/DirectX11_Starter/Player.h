#pragma once
#include "DirectXGame.h"
#include "GameEntity.h"
#include <vector>
#include "BoundingBox.h"
#include "PowerUp.h"
#include "Obstacle.h"

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
	Obstacle *getRocket();
	void setRocket(Mesh*mesh, Material*material);
	void reset();

	int getNumRockets();
	void setNumRockets(int num);
	bool getIsMoving();
	int getDirection();
	int getNumWatches();
	void setNumWatches(int num);

private:
	vector<float> railPosX;

	Rail currentRail;
	Rail targetRail;

	int direction;

	bool animateMovement;
	bool fireRocket;
	Obstacle * rocket;
	int numRockets;
	int numWatches;

	float horizontalSpeed;

};

