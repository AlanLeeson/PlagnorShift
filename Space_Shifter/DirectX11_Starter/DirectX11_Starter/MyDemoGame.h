#pragma once

#include <DirectXMath.h>
#include "DirectXGame.h"
#include "Camera.h"
#include "ChunkManager.h"
#include "GameEntity.h"
#include "GameStates.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Player.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "VSConstantBufferLayout.h"
#include "WICTextureLoader.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;

// Demo class which extends the base DirectXGame class
class MyDemoGame : public DirectXGame
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo
	void loadResources(void);
	void loadMeshes(void);
	void LoadShadersAndInputLayout();
	void loadTextures(void);
	void createMaterials(void);
	void createEntities(void);
	void InitializeCameraMatrices();

private:
	ResourceManager* resource_manager;
	RenderManager* render_manager;
	ChunkManager* chunk_manager;

	Camera* camera;
	Camera* gameCamera;

	DirectionalLight d_light01;
	DirectionalLight d_light02;
	bool whichCam;

	Material* simpleMat_wood;
	Material* simpleMat_metal;

	Mesh* torus;
	Mesh* cube;

	GameEntity* e_torus;
	GameEntity* e_cube;
	GameEntity* e_tile;

	// Buffers to hold actual geometry
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// A few more odds and ends we'll need
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBufferLayout dataToSendToVSConstantBuffer;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};