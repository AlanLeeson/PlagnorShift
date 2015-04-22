#pragma once
#include "DirectXGame.h"
#include "GameEntity.h"
#include <vector>

class Obstacle :
	public GameEntity
{
public:
	Obstacle(Mesh*, Material*);
	~Obstacle(void);

	void Update(float dt);

private:
	vector<float> positions;
	bool animateMovement;
	float speed;

	float startPosZ;
	float finishPosZ;

	void ResetLocation(void);
};