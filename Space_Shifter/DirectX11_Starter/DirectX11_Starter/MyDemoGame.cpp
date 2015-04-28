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

	sampler->Release();
	renderTargetView->Release();
	rtSRV->Release();
	rtView->Release();

	bloomTexture_original->Release();
	rtv_original->Release();
	rtsrv_original->Release();

	bloomTexture_highlight->Release();
	rtv_highlight->Release();
	rtsrv_highlight->Release();

	bloomTexture_gaussH->Release();
	rtv_gaussH->Release();
	rtsrv_gaussH->Release();

	bloomTexture_gaussV->Release();
	rtv_gaussV->Release();
	rtsrv_gaussV->Release();

	delete fullscreenQuad;
	fullscreenQuad = nullptr;

	delete e_rail;
	e_rail = nullptr;
	delete e_rail2;
	e_rail2 = nullptr;
	delete e_rail3;
	e_rail3 = nullptr;

	delete player;
	player = nullptr;

	delete obstacleManager;
	obstacleManager = nullptr;

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

	p_light01.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	p_light01.DiffuseColor = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	p_light01.Position = XMFLOAT3(2.0f, 0.0f, 6.0f);
	p_light01.Distance = 2.0f;
	render_manager->addPointLight(&p_light01);

	//p_light02.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//p_light02.DiffuseColor = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//p_light02.Position = XMFLOAT3(-5.0f, 0.0f, 0.0f);
	//p_light02.Distance = 5.0f;
	//render_manager->addPointLight(&p_light02);

	chunk_manager->buildChunk();

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::createEntities()
{
	Vertex quad[4];
	quad[0].Position = XMFLOAT3(-1, +1, 0);
	quad[1].Position = XMFLOAT3(-1, -1, 0);
	quad[2].Position = XMFLOAT3(+1, -1, 0);
	quad[3].Position = XMFLOAT3(+1, +1, 0);
	quad[0].UV = XMFLOAT2(0, 0);
	quad[1].UV = XMFLOAT2(0, 1);
	quad[2].UV = XMFLOAT2(1, 1);
	quad[3].UV = XMFLOAT2(1, 0);

	unsigned int indices[6];
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	fullscreenQuad = new Mesh(quad, 4, indices, 6, device);

	resource_manager->getMesh("rail", &rail);
	resource_manager->getMesh("racer", &racer);
	resource_manager->getMesh("obstacle", &m_obstacle);

	e_rail = new GameEntity(rail, railTexture);
	render_manager->addEntity(e_rail);
	e_rail2 = new GameEntity(rail, railTexture);
	render_manager->addEntity(e_rail2);
	e_rail3 = new GameEntity(rail, railTexture);
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

	// Player
	player = new Player(racer, simpleMat_racer);
	render_manager->addEntity(player);

	// Obstacles
	obstacleManager = new ObstacleManager(5, m_obstacle, obstacleTexture);
	vector<Obstacle*> obstacles = obstacleManager->GetObstacles();
	for (int i = 0; i < obstacleManager->GetCount(); i++)
	{
		render_manager->addEntity(obstacles[i]);
	}
}

void MyDemoGame::createMaterials()
{
	resource_manager->loadMaterial("Default_Diffuse", "Default_Diffuse", "RailTexture", "RailTexture");
	resource_manager->getMaterial("RailTexture", &railTexture);

	resource_manager->loadMaterial("Default_Diffuse", "Default_Diffuse", "racer", "racer");
	resource_manager->getMaterial("racer", &simpleMat_racer);

	resource_manager->loadMaterial("Default_Diffuse", "Default_Diffuse", "wood", "obstacle");
	resource_manager->getMaterial("obstacle", &obstacleTexture);
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
	resource_manager->loadMesh("cube.obj", "obstacle");
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{
	resource_manager->loadVertexShader("VertexShader.cso", "Default_Diffuse");
	resource_manager->loadPixelShader("PixelShader.cso", "Default_Diffuse");

	resource_manager->loadVertexShader("QuadVS.cso", "FullScreenQuad");
	resource_manager->loadPixelShader("QuadPS.cso", "FullScreenQuad");

	resource_manager->loadPixelShader("Bloom_highlight_PS.cso", "Bloom_Highlight");
	resource_manager->loadPixelShader("Gauss_blur_horizontal_PS.cso", "Gauss_Horizontal");
	resource_manager->loadPixelShader("Gauss_blur_vertical_PS.cso", "Gauss_Vertical");

	resource_manager->getVertexShader("FullScreenQuad", &quadVS);
	resource_manager->getPixelShader("FullScreenQuad", &quadPS);

	resource_manager->getPixelShader("Bloom_Highlight", &bloom_highlightPS);
	resource_manager->getPixelShader("Gauss_Horizontal", &bloom_gauss_hPS);
	resource_manager->getPixelShader("Gauss_Vertical", &bloom_gauss_vPS);
}

void MyDemoGame::loadTextures()
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
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

	HR(device->CreateSamplerState(&sd, &sampler));

	resource_manager->loadTexture("WoodFine0031_19_S.jpg", sd, "wood");
	resource_manager->loadTexture("MetalBare0144_1_S.jpg", sd, "metal");
	resource_manager->loadTexture("RailTexture.png", sd, "RailTexture");
	resource_manager->loadTexture("plagnortex.png", sd, "racer");

	//Render target setup
	D3D11_TEXTURE2D_DESC rtDesc;
	rtDesc.Width = windowWidth;
	rtDesc.Height = windowHeight;
	rtDesc.ArraySize = 1;
	rtDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	rtDesc.CPUAccessFlags = 0;
	rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtDesc.MipLevels = 1;
	rtDesc.MiscFlags = 0;
	rtDesc.SampleDesc.Count = 1;
	rtDesc.SampleDesc.Quality = 0;
	rtDesc.Usage = D3D11_USAGE_DEFAULT;

	device->CreateTexture2D(&rtDesc, 0, &renderTexture);
	device->CreateTexture2D(&rtDesc, 0, &bloomTexture_original);
	device->CreateTexture2D(&rtDesc, 0, &bloomTexture_highlight);
	device->CreateTexture2D(&rtDesc, 0, &bloomTexture_gaussH);
	device->CreateTexture2D(&rtDesc, 0, &bloomTexture_gaussV);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = rtDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(renderTexture, &rtvDesc, &rtView);
	device->CreateRenderTargetView(bloomTexture_original, &rtvDesc, &rtv_original);
	device->CreateRenderTargetView(bloomTexture_highlight, &rtvDesc, &rtv_highlight);
	device->CreateRenderTargetView(bloomTexture_gaussH, &rtvDesc, &rtv_gaussH);
	device->CreateRenderTargetView(bloomTexture_gaussV, &rtvDesc, &rtv_gaussV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = rtDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	device->CreateShaderResourceView(renderTexture, &srvDesc, &rtSRV);
	device->CreateShaderResourceView(bloomTexture_original, &srvDesc, &rtsrv_original);
	device->CreateShaderResourceView(bloomTexture_highlight, &srvDesc, &rtsrv_highlight);
	device->CreateShaderResourceView(bloomTexture_gaussH, &srvDesc, &rtsrv_gaussH);
	device->CreateShaderResourceView(bloomTexture_gaussV, &srvDesc, &rtsrv_gaussV);

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
	if (whichCam){
		camera->Update(dt);
	}
	//gameCamera->Update(dt);
	if (GetAsyncKeyState('A') & 0x8000)
	{
		player->move(-1);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		player->move(1);
	}

	player->Update(dt);
    p_light01.Position = player->getPosition();
	obstacleManager->Update(dt);
	
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.2f, 0.2f, 0.4f, 0.0f};
	const float blurColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Set up the input assembler
	//  - These technically don't need to be set every frame, unless you're changing the
	//    input layout (different kinds of vertices) or the topology (different primitives)
	//    between draws
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Render to texture
	deviceContext->OMSetRenderTargets(1, &rtv_original, depthStencilView);
	deviceContext->ClearRenderTargetView(rtv_original, color);

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//Renders all game entities with a specific camera
	if (whichCam){
		render_manager->renderAll(camera);
	}
	else{
		render_manager->renderAll(gameCamera);
	}

	quadVS->SetShader();

	// Set data
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vb = fullscreenQuad->getVertexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	deviceContext->IASetIndexBuffer(fullscreenQuad->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//Get the highlights for the bloom shader
	deviceContext->OMSetRenderTargets(1, &rtv_highlight, 0);
	deviceContext->ClearRenderTargetView(rtv_highlight, blurColor);

	bloom_highlightPS->SetFloat("threshold", 0.5f);
	bloom_highlightPS->SetSamplerState("basicSampler", sampler);
	bloom_highlightPS->SetShaderResourceView("sceneTexture", rtsrv_original);
	bloom_highlightPS->SetShader();

	// Draw
	deviceContext->DrawIndexed(fullscreenQuad->getIndexCount(), 0, 0);

	//Blur highlights horizontally
	deviceContext->OMSetRenderTargets(1, &rtv_gaussH, 0);
	deviceContext->ClearRenderTargetView(rtv_gaussH, blurColor);

	bloom_gauss_hPS->SetFloat2("pixelSize", XMFLOAT2(1.0f / windowWidth, 1.0f / windowHeight));
	bloom_gauss_hPS->SetSamplerState("basicSampler", sampler);
	bloom_gauss_hPS->SetShaderResourceView("diffuseTexture", rtsrv_highlight);
	bloom_gauss_hPS->SetShader();

	// Draw
	deviceContext->DrawIndexed(fullscreenQuad->getIndexCount(), 0, 0);

	//Blur highlights vertically
	deviceContext->OMSetRenderTargets(1, &rtv_gaussV, 0);
	deviceContext->ClearRenderTargetView(rtv_gaussV, blurColor);

	bloom_gauss_vPS->SetFloat2("pixelSize", XMFLOAT2(1.0f / windowWidth, 1.0f / windowHeight));
	bloom_gauss_vPS->SetSamplerState("basicSampler", sampler);
	bloom_gauss_vPS->SetShaderResourceView("diffuseTexture", rtsrv_gaussH);
	bloom_gauss_vPS->SetShader();

	// Draw
	deviceContext->DrawIndexed(fullscreenQuad->getIndexCount(), 0, 0);

	// Go back to the regular "back buffer"
	deviceContext->OMSetRenderTargets(1, &renderTargetView, 0);
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	quadPS->SetSamplerState("basicSampler", sampler);
	quadPS->SetShaderResourceView("diffuseTexture", rtsrv_gaussV);
	quadPS->SetShaderResourceView("sceneTexture", rtsrv_original);
	quadPS->SetInt("blurAmount", 0);
	quadPS->SetFloat2("pixelSize", XMFLOAT2(1.0f / windowWidth, 1.0f / windowHeight));
	quadPS->SetShader();

	// Draw
	deviceContext->DrawIndexed(fullscreenQuad->getIndexCount(), 0, 0);

	// Stick with the back buffer, but re-enable the depth buffer from earlier
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Unset the shader resource
	ID3D11ShaderResourceView* unset[2] = { 0, 0 };
	deviceContext->PSSetShaderResources(0, 2, unset);
	deviceContext->OMSetRenderTargets(0, 0, 0);

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

	//SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	//ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	float diffX = (float)(x - prevMousePos.x);
	//camera->rotateCameraPitch(diffX);
	float diffY = (float)(y - prevMousePos.y);
	//camera->rotateCameraRoll(diffY);

	prevMousePos.x = x;
	prevMousePos.y = y;


}
#pragma endregion