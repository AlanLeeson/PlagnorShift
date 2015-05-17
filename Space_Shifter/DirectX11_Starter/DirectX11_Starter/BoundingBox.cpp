#include "BoundingBox.h"

BoundingBox::BoundingBox(string instanceName, XMFLOAT3 pos, XMFLOAT3 s)
{
	position = pos;
	size = s;
	name = instanceName;
}
/*
BoundingBox::BoundingBoxClass(BoundingBoxClass const& other)
{
	this->centroid = other.centroid;
	this->name = other.name;
	this->min = other.min;
	this->max = other.max;
	this->modelToWorld = other.modelToWorld;
}*/

BoundingBox::~BoundingBox()
{

}
/*
XMFLOAT3 BoundingBoxClass::GetCentroid(void){ return centroid; }
XMFLOAT3 BoundingBoxClass::GetMinimum(void){ return min; }
XMFLOAT3 BoundingBoxClass::GetMaximum(void){ return max; }

XMFLOAT4X4 BoundingBoxClass::GetModelMatrix(void){ return modelToWorld; }

string BoundingBoxClass::GetName(void){ return name; }

void BoundingBoxClass::Calculate(vector<Vertex> vertices)
{
	int nVertices = static_cast<int>(vertices.size());
	
	if(nVertices == 0)
		return;

	//Go one by one on each component and realize which one is the smallest one
	if(nVertices > 0)
	{
		//Assume the first vertex is the smallest one
		min = vertices[0].Position;
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vertices[nVertex].Position.x < min.x)
				min.x = vertices[nVertex].Position.x;

			if(vertices[nVertex].Position.y < min.y)
				min.y = vertices[nVertex].Position.y;

			if(vertices[nVertex].Position.z < min.z)
				min.z = vertices[nVertex].Position.z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		max = vertices[0].Position;
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vertices[nVertex].Position.x > max.x)
				max.x = vertices[nVertex].Position.x;

			if(vertices[nVertex].Position.y > max.y)
				max.y = vertices[nVertex].Position.y;

			if(vertices[nVertex].Position.z > max.z)
				max.z = vertices[nVertex].Position.z;
		}
	}

	//The centroid is going to be the point that is halfway of the min to the max
	float centroidX = (min.x + max.x) / 2.0f;
	float centroidY = (min.y + max.y) / 2.0f;
	float centroidZ = (min.z + max.z) / 2.0f;

	centroid = XMFLOAT3(centroidX, centroidY, centroidZ);

	return;
	}
*/