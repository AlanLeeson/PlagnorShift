#include "Player.h"

Player::Player(Mesh* mesh, Material* material)
:GameEntity(mesh, material)
{
	currentRail = Middle;
	targetRail = Middle;

	animateMovement = false;
	fireRocket = false;
	horizontalSpeed = 10.0f;
	numRockets = 5;
	railPosX = { -2.0f, 1.3f, 4.5f };

	this->setScale(0.6f, 0.6f, 0.6f);
	this->setPosition(railPosX[this->currentRail], -0.75f, 3.0f);
	this->rotate(0.0f, 0.0f, 0.05f);
	XMFLOAT3 curPos = this->getPosition();
	XMFLOAT3 newPos = XMFLOAT3(curPos.x - 1.0f, curPos.y, curPos.z); 
	this->boundingBox = new BoundingBox("player", newPos, XMFLOAT3(0.8, 1, 1));
}
Player::~Player()
{
}

int Player::getNumRockets()
{
	return numRockets;
}

void Player::setNumRockets(int num)
{
	numRockets = num;
}

void Player::setRocket(Mesh*mesh, Material*material)
{
	rocket = new PowerUp(mesh, material, "rocket");
	rocket->SetActive(false);
	rocket->setRotation(1.5f, 0.0f, 0.0f);
	//rocket->setPosition(-3.0f, 4.0f, 2.0f);
}

bool Player::getIsMoving()
{
	return this->animateMovement;
}

int Player::getDirection()
{
	return direction;
}

Obstacle * Player::getRocket()
{
	return rocket;
}

void Player::move(int direction)
{ 
	if (direction == 0  || animateMovement)
		return;

	// Determine the target rail
	switch (currentRail)
	{
		case Left:
			targetRail = direction >= 1 ? Middle : currentRail;
			break;
		case Middle:
			targetRail = direction >= 1 ? Right : Left;
			break;
		case Right:
			targetRail = direction >= 1 ? currentRail : Middle;
			break;
	}

	animateMovement = true;
}

void Player::Update(float dt)
{
	if (rocket->Active()){
		rocket->fireRocket(dt);
	}
	if (animateMovement)
	{
		// Don't animate if you are already on your target rail
		if (currentRail == targetRail)
		{
			animateMovement = false;
			return;
		}

		// Move the player based on his direction
		direction = targetRail > currentRail ? 1 : -1;
		GameEntity::move(horizontalSpeed * direction * dt, 0.0f, 0.0f);

		XMFLOAT3 position = GameEntity::getPosition();
		float targetX = railPosX[targetRail];

		// Check which direction the player is moving
		if (direction == 1)
		{
			// Has the player reached or passed the target
			if (position.x >= targetX)
			{
				GameEntity::setPosition(targetX, position.y, position.z);
				currentRail = targetRail;
				animateMovement = false;
			}
		}
		else
		{
			// Has the player reached or passed the target
			if (position.x <= targetX)
			{
				GameEntity::setPosition(targetX, position.y, position.z);
				currentRail = targetRail;
				animateMovement = false;
			}
		}
	}

	if (numRockets > 0 && GetAsyncKeyState(VK_SPACE) < 0 && !rocket->Active())
	{
		rocket->SetActive(true);
		fireRocket = true;
		XMFLOAT3 position = GameEntity::getPosition();
		rocket->setPosition(position.x-1.45f,position.y+0.8f,position.z);
		numRockets--;
	}
}