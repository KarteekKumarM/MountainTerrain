#include "MT_Window.h"

static const LPCWSTR k_MountainTerrainWindow_ClassName = L"MountainTerrainWindowClass";
static const LPCWSTR k_MountainTerrainWindow_Title = L"Mountain Terran - By Karteek Mekala";
static const UINT k_MountainTerrainWindow_X_Pos = 300; 
static const UINT k_MountainTerrainWindow_Y_Pos = 300; 
static const UINT k_MountainTerrainWindow_Width = 800;
static const UINT k_MountainTerrainWindow_Height = 600;
static const DWORD k_MountainTerrainWindow_Style = WS_OVERLAPPEDWINDOW;

void MT_Window::Init(HINSTANCE hInstance, int nCmdShow) {
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
					   wc.lpszClassName = k_MountainTerrainWindow_ClassName;

					   // register the window class
					   RegisterClassEx(&wc);

					   // adjust windows heights so as to get target client window rect
					   RECT windowRect = {0, 0, k_MountainTerrainWindow_Width, k_MountainTerrainWindow_Height};
					   AdjustWindowRect(&windowRect, k_MountainTerrainWindow_Style, FALSE);
					   m_windowWidth = windowRect.right - windowRect.left;
					   m_windowHeight = windowRect.bottom - windowRect.top;

					   // create a window using the window class
					   hWnd = CreateWindowEx(NULL, // windows style 
											 k_MountainTerrainWindow_ClassName, // window class to use
											 k_MountainTerrainWindow_Title, // title of the window
											 k_MountainTerrainWindow_Style, // window style
											 k_MountainTerrainWindow_X_Pos, // x-pos of window
											 k_MountainTerrainWindow_Y_Pos, // y-pos of window
											 m_windowWidth, // width of window
											 m_windowHeight, // height of window
											 NULL, // parent window
											 NULL, // menu
											 hInstance, // app handle
											 NULL);

					   // show window
					   ShowWindow(hWnd, nCmdShow);

					   // initialize Renderer
					   m_renderer = new MT_Renderer();
					   m_renderer->Init(hWnd, m_windowWidth, m_windowHeight);
}

// message handler for the window
LRESULT CALLBACK MT_Window::WindowProc(HWND hWnd,
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

int MT_Window::EnterMessageLoop() {
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
		m_renderer->RenderFrame();
	}
	return msg.wParam;
}

void MT_Window::Clean() {
	m_renderer->Clean();
	delete m_renderer;
	m_renderer = 0;
}