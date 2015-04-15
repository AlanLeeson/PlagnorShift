// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);
	
	// If we can't initialize, we can't run
	if( !game.Init() )
		return 0;
	
	// All set to run the game
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;
}

MyDemoGame::~MyDemoGame()
{
	delete chunk_manager;
	chunk_manager = nullptr;

	delete camera;
	camera = nullptr;

	delete gameCamera;
	camera = nullptr;

	delete e_rail;
	e_rail = nullptr;
	delete e_rail2;
	e_rail2 = nullptr;
	delete e_rail3;
	e_rail3 = nullptr;


	delete e_racer;
	e_racer = nullptr;

	// Release all of the D3D stuff that's still hanging out
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);
	ReleaseMacro(vsConstantBuffer);
	ReleaseMacro(inputLayout);
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;

	resource_manager = &ResourceManager::getInstance();
	resource_manager->setDevice(device);
	resource_manager->setDeviceContext(deviceContext);

	render_manager = &RenderManager::getInstance();
	render_manager->setDevice(device);
	render_manager->setDeviceContext(deviceContext);

	chunk_manager = new ChunkManager();

	// Create the necessary DirectX buffers to draw something
	loadResources();
	
	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	createMaterials();
	createEntities();

	// Set up camera-related matrices
	InitializeCameraMatrices();
	whichCam = true;
	
	d_light01.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	d_light01.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	d_light01.Direction = XMFLOAT3(1.0f, -1.0f, 0.0f);
	render_manager->addDirectionalLight(&d_light01);

	d_light02.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	d_light02.DiffuseColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	d_light02.Direction = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	render_manager->addDirectionalLight(&d_light02);

	chunk_manager->buildChunk();

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::createEntities()
{
	resource_manager->getMesh("rail", &rail);
	resource_manager->getMesh("rail", &rail2);
	resource_manager->getMesh("rail", &rail3);
	resource_manager->getMesh("racer", &racer);

	e_rail = new GameEntity(rail, railTexture);
	render_manager->addEntity(e_rail);
	e_rail2 = new GameEntity(rail2, railTexture);
	render_manager->addEntity(e_rail2);
	e_rail3 = new GameEntity(rail3, railTexture);
	render_manager->addEntity(e_rail3);

	//position rails
	e_rail->rotate(0.0f, 1.55f, -0.05f);
	e_rail->setScale(2.0f, 1.0f, 1.0f);
	e_rail2->rotate(0.0f, 1.55f, -0.05f);
	e_rail2->setScale(2.0f, 1.0f, 1.0f);
	e_rail3->rotate(0.0f, 1.55f, -0.05f);
	e_rail3->setScale(2.0f, 1.0f, 1.0f);

	e_rail->setPosition(-3.0f, -2.0f, 12.0f);
	e_rail2->setPosition(0.5f, -2.0f, 12.0f);
	e_rail3->setPosition(3.5f, -2.0f, 12.0f);

	//all racer stuff
	e_racer = new GameEntity(racer, simpleMat_racer);
	render_manager->addEntity(e_racer);
	e_racer->setScale(0.6f, 0.6f, 0.6f);
	e_racer->setPosition(1.34, -1, 7);
	e_racer->rotate(0.0f, 0, 0.05f);
}

void MyDemoGame::createMaterials()
{
	resource_manager->loadMaterial("Default_Diffuse", "Default_Diffuse", "RailTexture", "RailTexture");

	resource_manager->getMaterial("RailTexture", &railTexture);


	resource_manager->loadMaterial("Default_Diffuse", "Default_Diffuse", "racer", "racer");
	resource_manager->getMaterial("racer", &simpleMat_racer);
}

void MyDemoGame::loadResources()
{
	loadMeshes();
	loadTextures();
}

void MyDemoGame::loadMeshes()
{
	resource_manager->loadMesh("rail.obj", "rail");
	resource_manager->loadMesh("rail.obj", "rail2");
	resource_manager->loadMesh("rail.obj", "rail3");

	resource_manager->loadMesh("racer.obj", "racer");
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{
	resource_manager->loadVertexShader("VertexShader.cso", "Default_Diffuse");
	resource_manager->loadPixelShader("PixelShader.cso", "Default_Diffuse");
}

void MyDemoGame::loadTextures()
{
	D3D11_SAMPLER_DESC sd;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.BorderColor = float(0.0f, 0.0f, 0.0f, 0.0f);
	sd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MinLOD = 0;
	sd.MipLODBias = 0;

	resource_manager->loadTexture("WoodFine0031_19_S.jpg", sd, "wood");
	resource_manager->loadTexture("MetalBare0144_1_S.jpg", sd, "metal");
	resource_manager->loadTexture("RailTexture.png", sd, "RailTexture");
	resource_manager->loadTexture("plagnortex.png", sd, "racer");
}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	camera = new Camera(AspectRatio());
	gameCamera = new Camera(AspectRatio());
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	// Update our projection matrix since the window size changed
	//Resizing hapens before camera is initialized?
	if (camera) camera->resize(AspectRatio());
	if (gameCamera) gameCamera->resize(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	// Take input, update game logic, etc.

	camera->Update(dt);
	//gameCamera->Update(dt);
	
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Set up the input assembler
	//  - These technically don't need to be set every frame, unless you're changing the
	//    input layout (different kinds of vertices) or the topology (different primitives)
	//    between draws
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Renders all game entities with a specific camera
	if (whichCam){
		render_manager->renderAll(camera);
	}
	else{
		render_manager->renderAll(gameCamera);
	}

	// Present the buffer
	//  - Puts the stuff on the screen
	//  - Do this EXACTLY once per frame
	//  - Always at the end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	whichCam = !whichCam;
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	float diffX = (float)(x - prevMousePos.x);
	camera->rotateCameraPitch(diffX);
	float diffY = (float)(y - prevMousePos.y);
	camera->rotateCameraRoll(diffY);

	//gameCamera->rotateCameraPitch(0);
	//gameCamera->rotateCameraRoll(0);

	prevMousePos.x = x;
	prevMousePos.y = y;


}
#pragma endregion