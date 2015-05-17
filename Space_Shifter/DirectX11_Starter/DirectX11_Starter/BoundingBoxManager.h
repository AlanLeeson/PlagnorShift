#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "BoundingBox.h"
#include <vector>

using namespace std;
using namespace DirectX;

class BoundingBoxManager
{
	//int numBoxes;
	//vector<string> colliding;

public:
	static BoundingBoxManager& getInstance(void);
	//int GetNumberOfBoxes(void);

	//void AddBox(string instanceName);
	//void RemoveBox(string instanceName = "ALL");

	//void SetModelMatrix(XMFLOAT4X4 modelMatrix, string instanceName = "ALL");
	void Update(void);
	bool checkCollision(BoundingBox*, BoundingBox*);
	vector<BoundingBox*> boundingBoxes;

private:
	BoundingBoxManager();
	BoundingBoxManager(BoundingBoxManager const&) = delete;
	~BoundingBoxManager();

	//void operator=(BoundingBoxManager const&) = delete;

	//void Init(void);
	
	//static BoundingBoxManager* instance;
	//void CollisionCheck(void);
	//void CollisionResponse(void);
	//
	//bool CheckForNameInList(string name);
};