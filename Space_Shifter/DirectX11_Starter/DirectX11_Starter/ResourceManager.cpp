#include "ResourceManager.h"


ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

ResourceManager::ResourceManager()
{
}

/*
	Destructor
*/
ResourceManager::~ResourceManager()
{
	std::map<char*, SimpleVertexShader*>::iterator it_v_shader;
	for (it_v_shader = v_shaders.begin(); it_v_shader != v_shaders.end(); it_v_shader++)
	{
		SimpleVertexShader* v = it_v_shader->second;
		delete v;
		v = nullptr;
	}
	v_shaders.clear();

	std::map<char*, SimplePixelShader*>::iterator it_p_shader;
	for (it_p_shader = p_shaders.begin(); it_p_shader != p_shaders.end(); it_p_shader++)
	{
		SimplePixelShader* p = it_p_shader->second;
		delete p;
		p = nullptr;
	}
	p_shaders.clear();

	std::map<char*, ID3D11ShaderResourceView*>::iterator it_tex;
	for (it_tex = textures.begin(); it_tex != textures.end(); it_tex++)
	{
		ID3D11ShaderResourceView* srv = it_tex->second;
		ReleaseMacro(srv);
	}
	textures.clear();

	std::map<char*, ID3D11SamplerState*>::iterator it_sample;
	for (it_sample = samplers.begin(); it_sample != samplers.end(); it_sample++)
	{
		ID3D11SamplerState* ss = it_sample->second;
		ReleaseMacro(ss);
	}
	samplers.clear();

	std::map<char*, Mesh*>::iterator it_mesh;
	for (it_mesh = meshes.begin(); it_mesh != meshes.end(); it_mesh++)
	{
		Mesh* mesh = it_mesh->second;
		delete mesh;
		mesh = nullptr;
	}
	meshes.clear();

	std::map<char*, Material*>::iterator it_mat;
	for (it_mat = materials.begin(); it_mat != materials.end(); it_mat++)
	{
		Material* mat = it_mat->second;
		delete mat;
		mat = nullptr;
	}
	materials.clear();

}

/*
	Set the device the manager will be using
	args:
	device -- The ID3D11Device
*/
void ResourceManager::setDevice(ID3D11Device* device)
{
	this->device = device;
}

/*
	Set the device context the manager will be using
	args:
	deviceContext -- The ID3D11DeviceContext
*/
void ResourceManager::setDeviceContext(ID3D11DeviceContext* deviceContext)
{
	this->deviceContext = deviceContext;
}

/*
	Load a vertex shader file and store it in a map

	args:
	file -- The path to the file
	key -- The optional name to map to (0 for file arg)

	returns:
	If load was successful
*/
bool ResourceManager::loadVertexShader(char* file, char* key)
{
	SimpleVertexShader* vShader;
	vShader = new SimpleVertexShader(device, deviceContext);

	//Convert char* to wchar_t*
	//https://msdn.microsoft.com/en-us/library/ms235631(VS.80).aspx
	size_t origsize = strlen(file) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wc_file[newsize];
	mbstowcs_s(&convertedChars, wc_file, origsize, file, _TRUNCATE);

	vShader->LoadShaderFile(wc_file);

	//Determine which key name to use
	char* _key = key ? key : file;
	v_shaders.insert(std::pair<char*, SimpleVertexShader*>(_key, vShader));

	return true;
}

/*
	Get a vertex shader by name

	args:
	key -- The key to the vertex shader
	v -- The address of the vertex shader to write to

	returns:
	If get was successful
*/
bool ResourceManager::getVertexShader(char* key, SimpleVertexShader** v)
{
	*v = v_shaders[key];
	return true;
}

/*
	Load a pixel shader file and store it in a map

	args:
	file -- The path to the file
	key -- The optional name to map to (0 for file arg)

	returns:
	If load was successful
*/
bool ResourceManager::loadPixelShader(char* file, char* key)
{
	SimplePixelShader* pShader;
	pShader = new SimplePixelShader(device, deviceContext);

	//Convert char* to wchar_t*
	//https://msdn.microsoft.com/en-us/library/ms235631(VS.80).aspx
	size_t origsize = strlen(file) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wc_file[newsize];
	mbstowcs_s(&convertedChars, wc_file, origsize, file, _TRUNCATE);

	pShader->LoadShaderFile(wc_file);

	//Determine which key to use
	char* _key = key ? key : file;
	p_shaders.insert(std::pair<char*, SimplePixelShader*>(_key, pShader));

	return true;
}

/*
	Get a pixel shader by name

	args:
	key -- The key to the pixel shader
	p -- The address of the pixel shader to write to

	returns:
	If get was successful
*/
bool ResourceManager::getPixelShader(char* key, SimplePixelShader** p)
{
	*p = p_shaders[key];
	return true;
}

/*
	Load a texture file and store it to a map

	args:
	file -- The path to the file
	sd -- The D3D11_SAMPLER_DESC of how the texture will be stored
	key -- The optional name to map to (0 for file arg)

	returns:
	If load was successful
*/
bool ResourceManager::loadTexture(char* file, D3D11_SAMPLER_DESC sd, char* key)
{
	//Convert char* to wchar_t*
	//https://msdn.microsoft.com/en-us/library/ms235631(VS.80).aspx
	size_t origsize = strlen(file) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wc_file[newsize];
	mbstowcs_s(&convertedChars, wc_file, origsize, file, _TRUNCATE);

	//wchar_t* wc_file = convertCharToWchar(file);

	ID3D11ShaderResourceView* srv;
	CreateWICTextureFromFile(device, deviceContext, wc_file, 0, &srv, 0);

	//Set the map key to either the file path or a custom name
	char* _key = key ? key : file;

	textures.insert(std::pair<char*, ID3D11ShaderResourceView*>(_key, srv));

	//Create the textures sampler state and store it in a map too
	ID3D11SamplerState* sampler;
	HR(device->CreateSamplerState(&sd, &sampler));
	samplers.insert(std::pair<char*, ID3D11SamplerState*>(_key, sampler));

	return true;
}

/*
	Get a texture by name

	args:
	key -- The key to the texture
	srv -- The address of the D3D11ShaderResourceView to write to
	sampler -- The address of the ID3D11SamplerState to write to


	returns:
	If get was successful
*/
bool ResourceManager::getTexture(char* key, ID3D11ShaderResourceView** srv, ID3D11SamplerState** sampler)
{
	*srv = textures[key];
	*sampler = samplers[key];
	return true;
}

/*
	Load a material and store it to a map

	args:
	v_name -- The key to the vertex shader
	p_name -- The key to the pixel shader
	tex_name -- The key to the texture
	key -- The optional name to map to (0 for tex_name arg)


	returns:
	If load was successful
*/
bool ResourceManager::loadMaterial(char* v_name, char* p_name, char* tex_name, char* key)
{
	//Get all the components needed for a material
	SimpleVertexShader* v = v_shaders[v_name];
	SimplePixelShader* p = p_shaders[p_name];
	ID3D11ShaderResourceView* src = textures[tex_name];
	ID3D11SamplerState* sampler = samplers[tex_name];

	Material* mat = new Material(v, p, src, sampler);

	char* _key = key ? key : tex_name;
	materials.insert(std::pair<char*, Material*>(_key, mat));

	return true;
}

/*
	Get a material by name

	args:
	key -- The key to the material
	material -- The address of the Material to write to

	returns:
	If get was successful
*/
bool ResourceManager::getMaterial(char* key, Material** material)
{
	*material = materials[key];

	return true;
}

/*
	Load a mesh and store it to a map

	args:
	file -- the path to the file
	key -- The optional name to map to (0 for file arg)

	returns:
	If load was successful
*/
bool ResourceManager::loadMesh(char* file, char* key)
{
	Mesh* mesh = new Mesh(file, device);

	//Set the map key to either the file path or a custom name
	char* _key = key ? key : file;

	meshes.insert(std::pair<char*, Mesh*>(_key, mesh));
	return true;
}

/*
	Get a mesh by name
	args:
	key -- The key to the mesh
	mesh -- The address of the Mesh to write to


	returns:
	If get was successful
*/
bool ResourceManager::getMesh(char* key, Mesh** mesh)
{
	*mesh = meshes[key];
	return true;
}

/*
	*CURRENTLY UNUSED*
	Converts a char array into a wchar_t array (needed for some file loading)

	args:
	c -- The char array in need of being converted

	returns:
	The wchar_t array of characters
*/
wchar_t* ResourceManager::convertCharToWchar(char* c)
{
	//Convert char* to wchar_t*
	//https://msdn.microsoft.com/en-us/library/ms235631(VS.80).aspx
	size_t origsize = strlen(c) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wc[newsize];
	mbstowcs_s(&convertedChars, wc, origsize, c, _TRUNCATE);

	return wc;
}
