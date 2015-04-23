#pragma once
#include "GameEntity.h"
class LightEntity :
	public GameEntity
{
public:
	LightEntity(Mesh*, Material*);
	~LightEntity();

	void setShaderVariables();
};

