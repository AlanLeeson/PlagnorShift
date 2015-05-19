#pragma once

#include <vector>
#include "Camera.h"
#include "DirectXGame.h"
#include "GameEntity.h"
#include "Light.h"
#include "Material.h"

class RenderManager
{
public:
	static RenderManager& getInstance(void);

	void setDevice(ID3D11Device*);
	void setDeviceContext(ID3D11DeviceContext*);
	void setCamera(Camera*);
	void setEntity(GameEntity*);
	void setLights(std::vector<Light*>);
	void addEntity(GameEntity*);
	void addLight(Light*);
	void addDirectionalLight(DirectionalLight*);
	void addPointLight(PointLight*);

	Camera* getCamera(void);
	GameEntity* getEntity(void);
	std::vector<Light*> getLights(void);

	void render(GameEntity*, Camera*);
	void renderMask(GameEntity*, Camera*);
	void renderAll(Camera*);
	void renderAllWithShaders(SimpleVertexShader*, SimplePixelShader*);

private:
	RenderManager();
	RenderManager(RenderManager const&) = delete;
	~RenderManager();

	void operator=(RenderManager const&) = delete;

	TransformBuffer b_transform;
	LightingBuffer b_lighting;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	Camera* camera;
	GameEntity* entity;
	std::vector<GameEntity*> entities;
	std::vector<Light*> lights;
	//Temporary vector until different lights can be parsed out of one vector
	std::vector<DirectionalLight*> d_lights;
	std::vector<PointLight*> p_lights;

	void setBufferData(void);
};

