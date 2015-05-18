#include "BoundingBoxManager.h"

BoundingBoxManager& BoundingBoxManager::getInstance(void)
{
	static BoundingBoxManager instance;
	return instance;
}

BoundingBoxManager::BoundingBoxManager()
{
}


BoundingBoxManager::~BoundingBoxManager()
{
	
}

void BoundingBoxManager::Update()
{

}

bool BoundingBoxManager::checkCollision(BoundingBox* a, BoundingBox* b)
{
	if (!(a->collidable && b->collidable))
	{
		return false;
	}
	//check the X axis
	if (abs(a->position.x - b->position.x) < a->size.x + b->size.x)
	{
		//check the Y axis
		if (abs(a->position.y - b->position.y) < a->size.y + b->size.y)
		{
			//check the Z axis
			if (abs(a->position.z - b->position.z) < a->size.z + b->size.z)
			{
				return true;
			}
		}
	}

	return false;
}
/*
public static boolean checkCollision(ObjectBox a, ObjectBox b){
{
//check the X axis
if(Math.abs(a.getX() - b.getX()) < a.getSizeX() + b.getSizeX())
{
//check the Y axis
if(Math.abs(a.getY() - b.getY()) < a.getSizeY() + b.getSizeY())
{
//check the Z axis
if(Math.abs(a.getZ() - b.getZ()) < a.getSizeZ() + b.getSizeZ())
{
return true;
}
}
}

return false;
} */