#pragma once

#include <map>
#include "Camera.h"
#include "DirectXGame.h"
#include "GameEntity.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "WICTextureLoader.h"

/*
Only manages things that shouldn't change during gameplay
*/
class ResourceManager
{
public:
	static ResourceManager& getInstance();

	void setDevice(ID3D11Device* device);
	void setDeviceContext(ID3D11DeviceContext* deviceContext);

	bool loadVertexShader(char* file, char* key=0);
	bool getVertexShader(char* key, SimpleVertexShader** v);

	bool loadPixelShader(char* file, char* key=0);
	bool getPixelShader(char* key, SimplePixelShader** p);

	bool loadTexture(char* file, D3D11_SAMPLER_DESC sd, char* key=0);
	bool getTexture(char* key, ID3D11ShaderResourceView** srv, ID3D11SamplerState** sampler);

	bool loadMaterial(char* v_name, char* p_name, char* tex_name, char* key=0);
	bool getMaterial(char* key, Material** material);

	bool loadMesh(char* file, char* key=0);
	bool getMesh(char* key, Mesh** mesh);
private:
	ResourceManager();
	ResourceManager(ResourceManager const&) = delete;
	~ResourceManager();

	void operator=(ResourceManager const&) = delete;

	wchar_t* convertCharToWchar(char* c);

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	std::map<char*, SimpleVertexShader*> v_shaders;
	std::map<char*, SimplePixelShader*> p_shaders;
	std::map<char*, ID3D11ShaderResourceView*> textures;
	std::map<char*, ID3D11SamplerState*> samplers;
	std::map<char*, Material*> materials;
	std::map<char*, Mesh*> meshes;
};

