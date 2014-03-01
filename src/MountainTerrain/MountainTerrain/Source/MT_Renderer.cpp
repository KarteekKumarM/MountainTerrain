#include "MT_Renderer.h"

void MT_Renderer::Init(HWND hWnd, UINT screenWidth, UINT screenHeight) {
	// struct that holds info about swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1; // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;	// output window
	swapChainDesc.SampleDesc.Count = 4;	// multi samples
	swapChainDesc.Windowed = TRUE;	// Windowed or full screen
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// allow switching to and from full screen

	// create the device, swap chain and device context
	D3D11CreateDeviceAndSwapChain(  NULL,
									D3D_DRIVER_TYPE_HARDWARE,
									NULL,
									NULL,
									NULL,
									NULL,
									D3D11_SDK_VERSION,
									&swapChainDesc,
									&m_dxgiSwapChain,
									&m_d3dDevice,
									NULL,
									&m_d3dDeviceContext);

	// get address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address as the render target
	m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_d3dBackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (FLOAT)screenWidth;
	viewPort.Height = (FLOAT)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	m_d3dDeviceContext->RSSetViewports(1, &viewPort);

	// world, view projection matrix 
	m_constantBuffer = new MT_ConstantBuffer();
	m_constantBuffer->Init(m_d3dDevice, screenWidth, screenHeight);

	// Init Scene
	m_scene = new MT_Terrain();
	m_scene->Init(m_d3dDevice, m_d3dDeviceContext);
}

void MT_Renderer::ProcessCameraState(MT_Camera *camera) {
	// world, view projection matrix 
	m_constantBuffer->Update(m_d3dDeviceContext, camera);
}

void MT_Renderer::RenderFrame() {
	// clear the back buffer
	const float clearColor[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	m_d3dDeviceContext->ClearRenderTargetView(m_d3dBackBuffer, clearColor);

	// scene rendering
	m_scene->RenderFrame(m_d3dDeviceContext);

	// switch the back buffer and the front buffer
	m_dxgiSwapChain->Present(0, 0);
}

void MT_Renderer::Clean() {
	// switch to window from full screen
	m_dxgiSwapChain->SetFullscreenState(FALSE, NULL);

	m_constantBuffer->Clean();
	delete m_constantBuffer;
	m_constantBuffer = 0;

	m_scene->Clean();
	delete m_scene;
	m_scene = 0;

	// release COM objects
	if(m_dxgiSwapChain) m_dxgiSwapChain->Release();
	if(m_d3dBackBuffer) m_d3dBackBuffer->Release();
	if(m_d3dDevice) m_d3dDevice->Release();
	if(m_d3dDeviceContext) m_d3dDeviceContext->Release();
}