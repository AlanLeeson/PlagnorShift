#include "Player.h"

Player::Player(Mesh* mesh, Material* material)
:GameEntity(mesh, material)
{
	currentRail = Middle;
	targetRail = Middle;

	animateMovement = false;
	horizontalSpeed = 10.0f;

	railPosX = { -2.0f, 1.3f, 4.5f };

	this->setScale(0.6f, 0.6f, 0.6f);
	this->setPosition(railPosX[this->currentRail], -0.75f, 6.0f);
	this->rotate(0.0f, 0.0f, 0.05f);
}


Player::~Player()
{
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
	if (animateMovement)
	{
		// Don't animate if you are already on your target rail
		if (currentRail == targetRail)
		{
			animateMovement = false;
			return;
		}

		// Move the player based on his direction
		int direction = targetRail > currentRail ? 1 : -1;
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
}