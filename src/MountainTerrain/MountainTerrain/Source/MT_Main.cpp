#include <Windows.h>

#include "MT_Window.h"
#include "MT_StatsWindow.h"
#include "MT_Profiler.h"

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

	// create stats window
	MT_StatsWindow statsWindow;
	statsWindow.Init(hInstance, nCmdShow);

	MT_Profiler::shared()->SetStatsWindow(&statsWindow);

	// enter message loop
	int ret = window.EnterMessageLoop();

	return ret;
}