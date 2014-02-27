#include <Windows.h>
#include "MT_Window.h"

// Entry point for Windows program
int WINAPI WinMain(HINSTANCE hInstance, 
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, 
				   int nCmdShow
				   ) {
					   // create window
					   MT_Window *window = new MT_Window();
					   window->Init(hInstance, nCmdShow);

					   // enter message loop
					   int ret = window->EnterMessageLoop();

					   // clean up
					   window->Clean();
					   delete window;
					   window = 0;

					   return ret;
}