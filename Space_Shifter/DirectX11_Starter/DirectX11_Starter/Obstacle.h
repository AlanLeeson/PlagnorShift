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

	float Active(void);

	bool OutOfBounds(void);
	void SpeedUp(float amount);
	void SlowMotion();
	void reset();

	void SetActive(bool active);
	void ResetLocation(void);
	virtual void Update(float dt);
	virtual void fireRocket(float dt);

private:
	vector<float> positions;
	bool animateMovement;
	bool active;

	float speed;
	float startPosZ;
	float finishPosZ;
};