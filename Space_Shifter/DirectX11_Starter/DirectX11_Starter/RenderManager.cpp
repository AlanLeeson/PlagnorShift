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
	entities.clear();
	lights.clear();
	d_lights.clear();
	p_lights.clear();
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

void RenderManager::setLights(std::vector<Light*> lights)
{
	this->lights = lights;
}

void RenderManager::addEntity(GameEntity* entity)
{
	entities.push_back(entity);
}

void RenderManager::addLight(Light* light)
{
	lights.push_back(light);
}

void RenderManager::addDirectionalLight(DirectionalLight* d_light)
{
	d_lights.push_back(d_light);
}

void RenderManager::addPointLight(PointLight* p_light)
{
	p_lights.push_back(p_light);
}

Camera* RenderManager::getCamera(void)
{
	return this->camera;
}

GameEntity* RenderManager::getEntity(void)
{
	return this->entity;
}

std::vector<Light*> RenderManager::getLights(void)
{
	return this->lights;
}

void RenderManager::setBufferData(void)
{
	b_transform.World = entity->getWorldMatrix();
	b_transform.Projection = camera->getProjectionMatrix();
	b_transform.View = camera->getViewMatrix();


	b_lighting.d0 = *d_lights[0];
	b_lighting.d1 = *d_lights[1];

	b_lighting.p0 = *p_lights[0];
	//b_lighting.p1 = *p_lights[1];
	

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

void RenderManager::renderAll(Camera* renderCamera)
{
	for (int i = 0; i < entities.size(); i++)
	{
		GameEntity* entity = entities[i];
		render(entity, renderCamera);
	}
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
