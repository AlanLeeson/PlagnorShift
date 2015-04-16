#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "BoundingBoxClass.h"
#include <vector>

using namespace std;
using namespace DirectX;

class BoundingBoxManager
{
	int numBoxes;
	vector<BoundingBoxClass*> boundingBox;
	vector<string> colliding;

public:
	static BoundingBoxManager* GetInstance();
	int GetNumberOfBoxes(void);

	void AddBox(string instanceName);
	void RemoveBox(string instanceName = "ALL");

	void SetModelMatrix(XMFLOAT4X4 modelMatrix, string instanceName = "ALL");
	void Update(void);

private:
	BoundingBoxManager(void);
	BoundingBoxManager(BoundingBoxManager const& other);
	BoundingBoxManager& operator=(BoundingBoxManager const& other);
	~BoundingBoxManager(void);

	void Init(void);
	
	static BoundingBoxManager* instance;
	void CollisionCheck(void);
	void CollisionResponse(void);

	bool CheckForNameInList(string name);
};