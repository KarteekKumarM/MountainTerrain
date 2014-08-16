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

void PrintSingle(HDC hdc, UINT x, UINT y, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	std::string bufferString(buffer);
	std::wstring bufferStringW(bufferString.begin(), bufferString.end());
	TextOut(hdc,
		x, y,
		bufferStringW.c_str(), strlen(buffer));

	va_end(args);
}

void PrintStatistics(HDC hdc)
{
	MT_Profiler* profiler = MT_Profiler::shared();

	UINT x = 5, y = 30;
	UINT fontHeight = 25;

	PrintSingle(hdc, x, y, "Current FPS : %0.2f", profiler->GetCurrentFPS());

	y += fontHeight;
	PrintSingle(hdc, x, y, "-------------- STATS ------------------");
	
	for (size_t i = 0; i < PERF_STAT_COUNT; i++)
	{
		y += fontHeight;
		PerfStatType type = PerfStatType(i);
		PrintSingle(hdc, x, y, "%s: %0.0f", profiler->ProfGetStatName(type).c_str(), profiler->ProfGetStatValue(type));
	}

	y += fontHeight;
	PrintSingle(hdc, x, y, "-------------- TIMERS ------------------");

	for (size_t i = 0; i < PERF_TIMER_COUNT; i++)
	{
		y += fontHeight;
		PerfTimerType type = PerfTimerType(i);
		PrintSingle(hdc, x, y, "%s: %f ms", profiler->ProfGetTimerName(type).c_str(), profiler->ProfGetTimerValue(type));
	}
}

void MT_StatsWindow::Redraw()
{
	InvalidateRect(m_hWnd, NULL, true);
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
	m_hWnd = CreateWindowEx(NULL, // windows style 
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
	ShowWindow(m_hWnd, nCmdShow);
}

void MT_StatsWindow::Clean()
{
	//m_renderer.Clean();
}