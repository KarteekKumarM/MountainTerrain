#include "MT_Renderer.h"
#include "MT_Logger.h"
#include "MT_Utility.h"
#include "MT_Settings.h"

#include <wincodec.h>
#include <string>

#include "ScreenGrab.h"
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;

const bool k_AntiAliasingEnabled = ANTIALIASING_ENABLED;

bool MT_Renderer::SetupRasterizer() {
	HRESULT result = S_OK;

	// setup how the polygons will be drawn
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.AntialiasedLineEnable = k_AntiAliasingEnabled;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = m_wireFrameEnabled ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &m_d3dRasterizerState);

	if( FAILED(result))
	{
		MT_Logger::Log("Unable to create rasterizer state");
	}

	m_d3dDeviceContext->RSSetState(m_d3dRasterizerState);

	return SUCCEEDED(result);
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

void MT_Renderer::ProcessInput(MT_InputHandler *inputHandler)
{

	if (inputHandler->IsWireMeshToggleKeyPressed())
	{
		m_wireFrameEnabled = !m_wireFrameEnabled;
		SetupRasterizer();
	}

	m_scene->ProcessInput(inputHandler);
}

bool MT_Renderer::SetupBackBuffer() 
{
	HRESULT result = S_OK;

	// get address of the back buffer
	result = m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_BackBuffer);

	if( FAILED(result) )
	{
		MT_Logger::LogError( "Unable to get back buffer from swap chain" );
		return false;
	}

	// use the back buffer address as the render target
	m_d3dDevice->CreateRenderTargetView(m_BackBuffer, NULL, &m_d3dBackBufferRenderTargetView);
	//m_BackBuffer->Release(); pBackBuffer = 0;

	return SUCCEEDED(result);
}

bool MT_Renderer::SetupBlending()
{
	HRESULT result = S_OK;

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_d3dDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);

	if( FAILED(result) ) {
		MT_Logger::LogError("Unable to create blend state");
		return false;
	}

	return SUCCEEDED(result);
}

bool MT_Renderer::SetupDepthStencilBuffer(UINT screenWidth, UINT screenHeight)
{
	HRESULT hresult = S_OK;

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

	hresult = m_d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);

	if(FAILED(hresult)) {
		MT_Logger::LogError("Unable to create depth stencil buffer");
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
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

	if(FAILED(hresult)) {
		MT_Logger::LogError("Unable to create depth stencil state");
		return false;
	}

	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initailze the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hresult = m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	if(FAILED(hresult)) {
		MT_Logger::LogError("Unable to create depth stencil view");
		return false;
	}

	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBufferRenderTargetView, m_depthStencilView);

	return SUCCEEDED(hresult);
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
	swapChainDesc.SampleDesc.Count = 1;	// multi samples
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

	SetupBackBuffer();

	SetupDepthStencilBuffer(screenWidth, screenHeight);

	m_wireFrameEnabled = WIRE_MESH;
	SetupRasterizer();

	SetupViewPort(screenWidth, screenHeight);

	SetupBlending();

	// world, view projection matrix 
	m_constantBuffer.Init(m_d3dDevice, screenWidth, screenHeight);

	m_scene = new MT_Terrain();
	m_scene->Init(m_d3dDevice, m_d3dDeviceContext);

	m_profiler.Init();
}

void MT_Renderer::ProcessCameraState(MT_Camera *camera)
{
	// world, view projection matrix 
	m_constantBuffer.Update(m_d3dDeviceContext, camera);
}

void MT_Renderer::CaptureFrame()
{
	static char* lastTimeString = 0;

	char *timeChars;
	GetTimeString(&timeChars);

	// screen shot allowed only every second
	if (lastTimeString == 0 || strcmp(timeChars, lastTimeString) != 0)
	{
		lastTimeString = timeChars;

		std::string screenshotpath = std::string(SCREEN_SHOT_PATH) + "Screenshot " + std::string(timeChars) + ".jpg";
		std::wstring screenshotpathW(screenshotpath.begin(), screenshotpath.end());

		HRESULT hr = SaveWICTextureToFile(m_d3dDeviceContext, m_BackBuffer, GUID_ContainerFormatJpeg, screenshotpathW.c_str());
		if (FAILED(hr))
		{
			MT_Logger::LogError("Unable to perform screen grab");
			return;
		}
	}
}

void MT_Renderer::RenderFrame()
{
	// clear the back buffer
	const float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	m_d3dDeviceContext->ClearRenderTargetView(m_d3dBackBufferRenderTargetView, clearColor);

	// clear the depth and stencil buffer
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// scene rendering
	m_scene->RenderFrame(m_d3dDeviceContext);

	// switch the back buffer and the front buffer
	m_dxgiSwapChain->Present(0, 0);

	m_profiler.Frame();
	m_profiler.Display(m_d3dDevice, m_d3dDeviceContext);
}

void MT_Renderer::Clean() 
{
	// switch to window from full screen
	m_dxgiSwapChain->SetFullscreenState(FALSE, NULL);

	m_scene->Clean();
	delete m_scene;
	m_scene = 0;

	if(m_depthStencilView) m_depthStencilView->Release();
	if(m_depthStencilState) m_depthStencilState->Release();
	if(m_depthStencilBuffer) m_depthStencilBuffer->Release();

	// release COM objects
	if(m_d3dRasterizerState) m_d3dRasterizerState->Release();
	if(m_dxgiSwapChain) m_dxgiSwapChain->Release();
	if(m_d3dBackBufferRenderTargetView) m_d3dBackBufferRenderTargetView->Release();
	if(m_d3dDevice) m_d3dDevice->Release();
	if(m_d3dDeviceContext) m_d3dDeviceContext->Release();
}