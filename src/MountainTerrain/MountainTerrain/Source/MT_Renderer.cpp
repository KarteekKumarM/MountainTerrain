#include "MT_Renderer.h"

const bool k_AntiAliasingEnabled = false;
const bool k_WireFrameEnabled = false;

void MT_Renderer::SetupRasterizer() 
{
	// setup how the polygons will be drawn
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.AntialiasedLineEnable = k_AntiAliasingEnabled;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = k_WireFrameEnabled ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_d3dRasterizerState);

	m_d3dDeviceContext->RSSetState(m_d3dRasterizerState);
}

void MT_Renderer::SetupViewPort(UINT screenWidth, UINT screenHeight) 
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (FLOAT)screenWidth;
	viewPort.Height = (FLOAT)screenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	m_d3dDeviceContext->RSSetViewports(1, &viewPort);
}

void MT_Renderer::SetupBackBuffer() 
{
	// get address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	// use the back buffer address as the render target
	m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_d3dBackBuffer);
	pBackBuffer->Release();
	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBuffer, NULL);
}

void MT_Renderer::SetupDepthStencilBuffer(UINT screenWidth, UINT screenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT hresult = S_OK;

	hresult = m_d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hresult = m_d3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hresult = m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBuffer, m_depthStencilView);
}

void MT_Renderer::Init(HWND hWnd, UINT screenWidth, UINT screenHeight) 
{
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

	SetupRasterizer();

	SetupBackBuffer();

	SetupViewPort(screenWidth, screenHeight);

	SetupDepthStencilBuffer(screenWidth, screenHeight);

	// world, view projection matrix 
	m_constantBuffer = new MT_ConstantBuffer();
	m_constantBuffer->Init(m_d3dDevice, screenWidth, screenHeight);

	// Init Scene
	m_scene = new MT_Terrain();
	m_scene->Init(m_d3dDevice, m_d3dDeviceContext);
}

void MT_Renderer::ProcessCameraState(MT_Camera *camera)
{
	// world, view projection matrix 
	m_constantBuffer->Update(m_d3dDeviceContext, camera);
}

void MT_Renderer::RenderFrame()
{
	// clear the back buffer
	const float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	m_d3dDeviceContext->ClearRenderTargetView(m_d3dBackBuffer, clearColor);

	// scene rendering
	m_scene->RenderFrame(m_d3dDeviceContext);

	// switch the back buffer and the front buffer
	m_dxgiSwapChain->Present(0, 0);
}

void MT_Renderer::Clean() 
{
	// switch to window from full screen
	m_dxgiSwapChain->SetFullscreenState(FALSE, NULL);

	m_constantBuffer->Clean();
	delete m_constantBuffer;
	m_constantBuffer = 0;

	m_scene->Clean();
	delete m_scene;
	m_scene = 0;

	if(m_depthStencilView) m_depthStencilView->Release();
	if(m_depthStencilState) m_depthStencilState->Release();
	if(m_depthStencilBuffer) m_depthStencilBuffer->Release();

	// release COM objects
	if(m_d3dRasterizerState) m_d3dRasterizerState->Release();
	if(m_dxgiSwapChain) m_dxgiSwapChain->Release();
	if(m_d3dBackBuffer) m_d3dBackBuffer->Release();
	if(m_d3dDevice) m_d3dDevice->Release();
	if(m_d3dDeviceContext) m_d3dDeviceContext->Release();
}