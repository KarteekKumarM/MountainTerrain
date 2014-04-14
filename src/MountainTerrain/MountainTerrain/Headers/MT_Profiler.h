#ifndef MT_PROFILER_H
#define MT_PROFILER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class MT_StatsWindow;

class MT_Profiler
{
private:
	MT_StatsWindow *m_statsWindow;
	ULONGLONG m_timeAtLastSecond;
	ULONGLONG m_startTime;
	ULONGLONG m_totalFrames;
	UINT m_currentFPS;
	UINT m_frameCountInCurrentSecond;
	void Init();
public:
	void SetStatsWindow(MT_StatsWindow *statsWindow);
	FLOAT GetAverageFPS();
	UINT GetCurrentFPS();
	void RecordFrame();
	static MT_Profiler* shared();
};

#endif