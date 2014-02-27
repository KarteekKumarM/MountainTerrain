#include <Windows.h>

#include "MT_Renderer.h"

class MT_Window {
private:	
	UINT m_windowWidth, m_windowHeight;
	MT_Renderer *m_renderer;
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message,  WPARAM wParam, LPARAM lParam);
	void Init(HINSTANCE hInstance, int nCmdShow);
	int EnterMessageLoop();
	void Clean();
};