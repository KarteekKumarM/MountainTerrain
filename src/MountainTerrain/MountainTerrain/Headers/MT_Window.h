#ifndef MT_WINDOW_H
#define MT_WINDOW_H

#include <Windows.h>

#include "MT_Renderer.h"
#include "MT_InputHandler.h"

class MT_Window 
{
private:	
	UINT m_windowWidth, m_windowHeight;
	MT_Renderer m_renderer;
	MT_InputHandler *m_inputHandler;
	MT_Camera m_camera;
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message,  WPARAM wParam, LPARAM lParam);
	void Init(HINSTANCE hInstance, int nCmdShow);
	int EnterMessageLoop();
	void Clean();
};

#endif