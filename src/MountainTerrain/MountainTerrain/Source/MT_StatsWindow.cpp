#include "MT_StatsWindow.h"
#include "MT_Settings.h"

#include "MT_Profiler.h"

#include <tchar.h>
#include <string>

static const LPCWSTR k_StatsWindow_ClassName = STATS_WINDOW_NAME;
static const LPCWSTR k_StatsWindow_Title = STATS_WINDOW_TITLE;
static const UINT k_StatsWindow_X_Pos = STATS_WINDOW_POS_X;
static const UINT k_StatsWindow_Y_Pos = STATS_WINDOW_POS_Y;
static const UINT k_StatsWindow_Width = STATS_WINDOW_WIDTH;
static const UINT k_StatsWindow_Height = STATS_WINDOW_HEIGHT;
static const DWORD k_StatsWindow_Style = WS_OVERLAPPEDWINDOW;

void PrintStatistics(HDC hdc)
{
	/*TCHAR greeting[] = _T("Hello, World!");
	TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));*/

	MT_Profiler* profiler = MT_Profiler::shared();

	char currentFPS[512];
	sprintf_s(currentFPS, 512, "Current FPS : %d", profiler->GetCurrentFPS());
	std::string currentFPSString(currentFPS);
	std::wstring currentFPSStringW(currentFPSString.begin(), currentFPSString.end());

	TextOut(hdc,
		5, 5,
		currentFPSStringW.c_str(), strlen(currentFPS));
}

LRESULT CALLBACK MT_StatsWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			PrintStatistics(hdc);

			EndPaint(hWnd, &ps);
		}
			return 0;
		default:
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
void MT_StatsWindow::Init(HINSTANCE hInstance, int nCmdShow)
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
	wc.lpszClassName = k_StatsWindow_ClassName;

	// register the window class
	RegisterClassEx(&wc);

	// adjust windows heights so as to get target client window rect
	RECT windowRect = { 0, 0, k_StatsWindow_Width, k_StatsWindow_Height };
	AdjustWindowRect(&windowRect, k_StatsWindow_Style, FALSE);
	m_windowWidth = windowRect.right - windowRect.left;
	m_windowHeight = windowRect.bottom - windowRect.top;

	// create a window using the window class
	hWnd = CreateWindowEx(NULL, // windows style 
							k_StatsWindow_ClassName, // window class to use
							k_StatsWindow_Title, // title of the window
							k_StatsWindow_Style, // window style
							k_StatsWindow_X_Pos, // x-pos of window
							k_StatsWindow_Y_Pos, // y-pos of window
							m_windowWidth, // width of window
							m_windowHeight, // height of window
							NULL, // parent window
							NULL, // menu
							hInstance, // app handle
							NULL);
	ShowWindow(hWnd, nCmdShow);
}

void MT_StatsWindow::Clean()
{
	//m_renderer.Clean();
}