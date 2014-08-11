#include "MT_Window.h"
#include "MT_Settings.h"
#include "MT_Profiler.h"

static const LPCWSTR k_MountainTerrainWindow_ClassName = WINDOW_NAME;
static const LPCWSTR k_MountainTerrainWindow_Title = WINDOW_TITLE;
static const UINT k_MountainTerrainWindow_X_Pos = WINDOW_POS_X; 
static const UINT k_MountainTerrainWindow_Y_Pos = WINDOW_POS_Y;
static const UINT k_MountainTerrainWindow_Width = WINDOW_WIDTH;
static const UINT k_MountainTerrainWindow_Height = WINDOW_HEIGHT;
static const DWORD k_MountainTerrainWindow_Style = WS_OVERLAPPEDWINDOW;

void MT_Window::Init(HINSTANCE hInstance, int nCmdShow) 
{
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
					   ShowWindow(hWnd, nCmdShow);

					   m_camera.Init();
					   m_renderer.Init(hWnd, m_windowWidth, m_windowHeight);
					   m_inputHandler = new MT_InputHandler();
					   m_inputHandler->Init();
}

// message handler for the window
LRESULT CALLBACK MT_Window::WindowProc(HWND hWnd,
							UINT message, 
							WPARAM wParam, 
							LPARAM lParam)
{
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

int MT_Window::EnterMessageLoop()
{

	// enter the run loop
	MSG msg;
	while(TRUE) 
	{

		// track delta time
		ULONGLONG timeCur = GetTickCount64();
		static ULONGLONG timeStart = timeCur;
		ULONGLONG dTime = timeCur - timeStart;

		// handle messages
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke message to right format
			TranslateMessage(&msg);

			// send message to our input handler
			bool inputStateChanged = m_inputHandler->ProcessMessage(msg.message, msg.wParam, msg.lParam);
			if(inputStateChanged) 
			{
				m_camera.ProcessInput(m_inputHandler);
				m_renderer.ProcessInput(m_inputHandler);
			}

			// send message to WindowsProc callback
			DispatchMessage(&msg);
			// check if its time to quit
			if(msg.message == WM_QUIT)
			{
				break;
			}
		} 

		// render
		if (m_inputHandler->IsScreenGrabKeyPressed())
		{
			m_renderer.CaptureFrame();
			m_inputHandler->ResetScreenGrabKey();
		}
		m_renderer.ProcessCameraState(&m_camera);

		MT_Profiler::ProfBegin(PERF_RENDER);
		//MT_Profiler::Wait(0.015);
		m_renderer.RenderFrame();
		MT_Profiler::ProfEnd(PERF_RENDER);

		MT_Profiler::shared()->Update();
	}
	return msg.wParam;
}

void MT_Window::Clean()
{
	m_inputHandler->Clean();
	delete m_inputHandler;
	m_inputHandler = 0;
}