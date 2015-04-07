#include "RenderManager.h"

RenderManager& RenderManager::getInstance(void)
{
	static RenderManager instance;
	return instance;
}

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}

/*
	Set the device the manager will be using
*/
void RenderManager::setDevice(ID3D11Device* device)
{
	this->device = device;
}

/*
	Set the device context the manager will be using
*/
void RenderManager::setDeviceContext(ID3D11DeviceContext* deviceContext)
{
	this->deviceContext = deviceContext;
}

/*

*/
void RenderManager::setCamera(Camera* camera)
{
	this->camera = camera;
}

void RenderManager::setEntity(GameEntity* entity)
{
	this->entity = entity;
}

void RenderManager::setLights(std::vector<Light> lights)
{
	this->lights = lights;
}

void RenderManager::addLight(Light* light)
{
	lights.push_back(*light);
}

Camera* RenderManager::getCamera(void)
{
	return this->camera;
}

GameEntity* RenderManager::getEntity(void)
{
	return this->entity;
}

std::vector<Light> RenderManager::getLights(void)
{
	return this->lights;
}

void RenderManager::setBufferData(void)
{
	b_transform.World = entity->getWorldMatrix();
	b_transform.Projection = camera->getProjectionMatrix();
	b_transform.View = camera->getViewMatrix();

	Material* mat = entity->getMaterial();
	mat->mapShaderData(b_transform, b_lighting);
}

void RenderManager::render(GameEntity* renderEntity, Camera* renderCamera)
{
	this->entity = renderEntity;
	this->camera = renderCamera;

	setBufferData();

	entity->draw(device, deviceContext);
}

void RenderManager::renderAllWithShaders(SimpleVertexShader* vShader, SimplePixelShader* pShader)
{
	//Set the two shaders in the device context

	//Set the scene dependent data

	//Get all of the Materials that use that shader

	//Foreach m in materials
	//--Set the Material specific data

	//--Get all the GameEntities that use the material

	//--Foreach ge in gameEntities
	//----Set the GameEntity specific data

	//----Render the GameEntity
}
