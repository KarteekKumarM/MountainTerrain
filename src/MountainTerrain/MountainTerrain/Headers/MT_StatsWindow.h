#ifndef MT_STATS_WINDOW_H
#define MT_STATS_WINDOW_H

#include <Windows.h>

class MT_StatsWindow
{
private:
	UINT m_windowWidth, m_windowHeight;
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Init(HINSTANCE hInstance, int nCmdShow);
	void Clean();
};

#endif