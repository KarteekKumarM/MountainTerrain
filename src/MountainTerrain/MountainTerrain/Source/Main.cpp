#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "GlobalConstants.h"

// function prototypes MAIN
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void InitD3D(HWND hWnd);
void CleanD3D();
void RenderFrame(void);
HRESULT InitShaders();

// globals
IDXGISwapChain *dxgiSwapChain;
ID3D11Device *d3dDevice;
ID3D11DeviceContext *d3dDeviceContext;
ID3D11RenderTargetView *d3dBackBuffer;
ID3D11VertexShader *d3dVertexShader;
ID3D11PixelShader *d3dPixelShader;

// render funcs
void CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob);
void DrawFunc(ID3D11DeviceContext *d3dDeviceContext);
void CleanRender();
void LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);

// Entry point for Windows program
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nCmdShow
				   ) {

					   const LPCWSTR MountainTerrainWindow_ClassName = L"MountainTerrainWindowClass";
					   const LPCWSTR MountainTerrainWindow_Title = L"Mountain Terran - By Karteek Mekala";
					   const UINT MountainTerrainWindow_X_Pos = 300; 
					   const UINT MountainTerrainWindow_Y_Pos = 300; 
					   const UINT MountainTerrainWindow_Width = GC_SCREEN_WIDTH; 
					   const UINT MountainTerrainWindow_Height = GC_SCREEN_HEIGHT; 
					   const DWORD MountainTerrainWindow_Style = WS_OVERLAPPEDWINDOW;

					   // handle to the window
					   HWND hWnd;

					   // struct for information regarding the window class
					   WNDCLASSEX wc;
					   ZeroMemory(&wc, sizeof(WNDCLASSEX)); // clear out
					   wc.cbSize = sizeof(WNDCLASSEX);
					   wc.style = CS_HREDRAW | CS_VREDRAW;
					   wc.lpfnWndProc = WindowProc;
					   wc.hInstance = hInstance;
					   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
					   //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
					   wc.lpszClassName = MountainTerrainWindow_ClassName;

					   // register the window class
					   RegisterClassEx(&wc);

					   // adjust windows heights so as to get target client window rect
					   RECT windowRect = {0, 0, MountainTerrainWindow_Width, MountainTerrainWindow_Height};
					   AdjustWindowRect(&windowRect, MountainTerrainWindow_Style, FALSE);
					   int adjustedWindowWidth = windowRect.right - windowRect.left;
					   int adjustedWindowHeight = windowRect.bottom - windowRect.top;

					   // create a window using the window class
					   hWnd = CreateWindowEx(NULL, // windows style 
											 MountainTerrainWindow_ClassName, // window class to use
											 MountainTerrainWindow_Title, // title of the window
											 MountainTerrainWindow_Style, // window style
											 MountainTerrainWindow_X_Pos, // x-pos of window
											 MountainTerrainWindow_Y_Pos, // y-pos of window
											 adjustedWindowWidth, // width of window
											 adjustedWindowHeight, // height of window
											 NULL, // parent window
											 NULL, // menu
											 hInstance, // app handle
											 NULL);

					   // show window
					   ShowWindow(hWnd, nCmdShow);

					   InitD3D(hWnd);

					   // enter the run loop
					   MSG msg;
					   while(TRUE) {
						   // handle messages
						   if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
								// translate keystroke message to right format
								TranslateMessage(&msg);
								// send message to WindowsProc callback
								DispatchMessage(&msg);
								// check if its time to quit
								if(msg.message == WM_QUIT) {
									break;
								}
						   } 
						   // render
						   RenderFrame();
					   }

					   // clean direct3d
					   CleanD3D();

					   return msg.wParam;
}

// message handler for the window
LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT message, 
							WPARAM wParam, 
							LPARAM lParam) {

								switch (message)
								{
								case WM_DESTROY:
									PostQuitMessage(0);
									return 0;
								default:
									break;
								}
								return DefWindowProc(hWnd, message, wParam, lParam);
}

// initialize direct3d
void InitD3D(HWND hWnd) {

	// struct that holds info about swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1; // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32-bit color
	swapChainDesc.BufferDesc.Width = GC_SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = GC_SCREEN_HEIGHT;
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
									&dxgiSwapChain,
									&d3dDevice,
									NULL,
									&d3dDeviceContext);

	// get address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address as the render target
	d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &d3dBackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	d3dDeviceContext->OMSetRenderTargets(1, &d3dBackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = GC_SCREEN_WIDTH;
	viewPort.Height = GC_SCREEN_HEIGHT;
	d3dDeviceContext->RSSetViewports(1, &viewPort);

	// load shaders
	InitShaders();
	LoadVertexBuffer(d3dDevice, d3dDeviceContext);
}

// clean up direct3d
void CleanD3D() {

	// switch to window from full screen
	dxgiSwapChain->SetFullscreenState(FALSE, NULL);

	// clean up render work
	CleanRender();

	// release COM objects
	if(d3dVertexShader) d3dVertexShader->Release();
	if(d3dPixelShader) d3dPixelShader->Release();
	if(dxgiSwapChain) dxgiSwapChain->Release();
	if(d3dBackBuffer) d3dBackBuffer->Release();
	if(d3dDevice) d3dDevice->Release();
	if(d3dDeviceContext) d3dDeviceContext->Release();
}

// rendering work done here
void RenderFrame(void) {
	// clear the back buffer
	const float clearColor[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	d3dDeviceContext->ClearRenderTargetView(d3dBackBuffer, clearColor);

	// do 3d rendering on back buffer here
	DrawFunc(d3dDeviceContext);

	// switch the back buffer and the front buffer
	dxgiSwapChain->Present(0, 0);
}

// reusable function to compile vertex and pixel shaders
HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut) {
	HRESULT hr = S_OK;

	// setup shader flags
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// better shader debugging
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *errorBlob = nullptr;
	hr = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, shaderModel, dwShaderFlags, 0, blobOut, &errorBlob);
	if(FAILED(hr)) {
		if(errorBlob) {
			OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}
	}

	if(errorBlob) {
		errorBlob->Release();
	}

	return hr;
}

// load the vertex and pixel shaders
HRESULT InitShaders() {

	const LPCWSTR VERTEX_SHADER_FILENAME = L"Shaders/SimpleShader.hlsl";
	const LPCSTR VERTEX_SHADER_ENTRYPOINT = "VS";
	const LPCSTR VERTEX_SHADER_MODEL = "vs_5_0";

	const LPCWSTR PIXEL_SHADER_FILENAME = L"Shaders/SimpleShader.hlsl";
	const LPCSTR PIXEL_SHADER_ENTRYPOINT = "PS";
	const LPCSTR PIXEL_SHADER_MODEL = "ps_5_0";

	HRESULT hr = S_OK;

	// -------- VERTEX SHADER
	ID3DBlob *vertexShaderBlob;
	// compile vertex shader
	hr = CompileShaderFromFile(VERTEX_SHADER_FILENAME, VERTEX_SHADER_ENTRYPOINT, VERTEX_SHADER_MODEL, &vertexShaderBlob);
	if(FAILED(hr)) {
		MessageBox(nullptr, L"Unable to compile vertex shader", L"Error", MB_OK);
		return hr;
	} else {
		// load the shader and setup the shader obj
		d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &d3dVertexShader);

		// set the shader as active
		d3dDeviceContext->VSSetShader(d3dVertexShader, 0, 0);

		// create input layour for vertex buffer
		CreateInputLayoutObjectForVertexBuffer(d3dDevice, d3dDeviceContext, vertexShaderBlob);
	}

	// -------- PIXEL SHADER
	ID3DBlob *pixelShaderBlob;
	// compile pixel shader
	hr = CompileShaderFromFile(PIXEL_SHADER_FILENAME, PIXEL_SHADER_ENTRYPOINT, PIXEL_SHADER_MODEL, &pixelShaderBlob);
	if(FAILED(hr)) {
		MessageBox(nullptr, L"Unable to compile pixel shader", L"Error", MB_OK);
		return hr;
	} else {
		// load the shader and setup the shader obj
		d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &d3dPixelShader);

		// set the shader as active
		d3dDeviceContext->PSSetShader(d3dPixelShader, 0, 0);
	}	

	return hr;
}