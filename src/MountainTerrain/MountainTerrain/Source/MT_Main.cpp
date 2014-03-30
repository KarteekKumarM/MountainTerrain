#include <Windows.h>
#include "MT_Window.h"

// Entry point for Windows program
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nCmdShow
				   ) 
{
	// create window
	MT_Window window;
	window.Init(hInstance, nCmdShow);

	// enter message loop
	int ret = window.EnterMessageLoop();

	return ret;
}