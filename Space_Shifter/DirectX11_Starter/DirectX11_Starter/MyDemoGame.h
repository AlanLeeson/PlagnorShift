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
#include "Obstacle.h"
#include "ObstacleManager.h"
#include "PowerUp.h"
#include "BoundingBoxManager.h"

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
	void DrawMenu();

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

	void CreateGeometryBuffers();
	void DrawSpawn();
	void SwapSOBuffers();
private:
	ResourceManager* resource_manager;
	RenderManager* render_manager;
	ChunkManager* chunk_manager;
	BoundingBoxManager* bounding_box_manager;

	Camera* camera;
	Camera* gameCamera;

	DirectionalLight d_light01;
	DirectionalLight d_light02;
	PointLight p_light01;
	PointLight p_light02;
	bool whichCam;

	SimplePixelShader* menuPS;
	Material* menuMat;
	ID3D11SamplerState* menuSampler;

	Material* railTexture;
	Material* simpleMat_racer;
	Material* obstacleTexture;
	Material* powerUpTexture;

	ID3D11Texture2D* renderTexture;
	ID3D11RenderTargetView* rtView;
	ID3D11ShaderResourceView* rtSRV;

	Mesh* fullscreenQuad;
	ID3D11SamplerState* sampler;
	SimpleVertexShader* quadVS;
	SimplePixelShader* quadPS;

	SimplePixelShader* blurPS;
	SimplePixelShader* maskPS;

	ID3D11Texture2D* maskTexture;
	ID3D11RenderTargetView* rtv_mask;
	ID3D11ShaderResourceView* rtsrv_mask;

	ID3D11Texture2D* bloomTexture_highlight;
	ID3D11RenderTargetView* rtv_highlight;
	ID3D11ShaderResourceView* rtsrv_highlight;
	SimplePixelShader* bloom_highlightPS;

	ID3D11Texture2D* bloomTexture_gaussH;
	ID3D11RenderTargetView* rtv_gaussH;
	ID3D11ShaderResourceView* rtsrv_gaussH;
	SimplePixelShader* bloom_gauss_hPS;
	SimplePixelShader* bloom_gauss_vPS;

	ID3D11Texture2D* bloomTexture_gaussV;
	ID3D11RenderTargetView* rtv_gaussV;
	ID3D11ShaderResourceView* rtsrv_gaussV;

	ID3D11Texture2D* bloomTexture_original;
	ID3D11RenderTargetView* rtv_original;
	ID3D11ShaderResourceView* rtsrv_original;

	ID3D11Texture2D* bloomTexture_final;
	ID3D11RenderTargetView* rtv_final;
	ID3D11ShaderResourceView* rtsrv_final;

	SimpleVertexShader* bloomVS;
	SimplePixelShader* bloomPS;

	Mesh* rail;
	Mesh* racer;
	Mesh* m_powerUp;
	Mesh* pu_rocket;
	Mesh* pu_stopWatch;

	GameEntity* e_rail;
	GameEntity* e_rail2;
	GameEntity* e_rail3;

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

	Player* player;
	Obstacle* powerUp;
	Obstacle* rocket;
	Obstacle* stopWatch;

	Mesh* m_obstacle;
	ObstacleManager* obstacleManager;

	//buffer stuff for geometry shader
	ID3D11Buffer* soBufferRead;
	ID3D11Buffer* soBufferWrite;
	bool spawnFlip;
	int frameCount;
	//particle effect shaders
	SimpleVertexShader* engVertexShader;
	SimplePixelShader* engPixelShader;
	SimpleGeometryShader* engGeometryShader;
	SimpleVertexShader* engSpawnVS;
	SimpleGeometryShader* engSpawnGS;
	//random data for shaders
	ID3D11Texture1D* randomTexture;
	ID3D11ShaderResourceView* randomSRV;
	ID3D11SamplerState* randomSampler;
	//blend state for particles
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthState;
	//texture sampler and resource view
	ID3D11SamplerState * sampState; 
	ID3D11ShaderResourceView* shaderRV;

	// Particle params
	XMFLOAT3 particleStartPosition;
	XMFLOAT3 particleStartVelocity;
	XMFLOAT4 particleStartColor;
	XMFLOAT4 particleMidColor;
	XMFLOAT4 particleEndColor;
	float particleStartSize;
	float particleMidSize;
	float particleEndSize;
	float particleAgeToSpawn;
	float particleMaxLifetime;
	XMFLOAT3 particleConstantAccel;

	//for testing
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
};