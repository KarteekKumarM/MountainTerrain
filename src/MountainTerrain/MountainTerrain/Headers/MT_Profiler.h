#ifndef MT_PROFILER_H
#define MT_PROFILER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <string>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class MT_StatsWindow;

enum PerfTimerType
{
	PERF_RENDER,
	PERF_RENDER_SKYBOX,
	PERF_RENDER_TERRAIN,
	PERF_UPDATE_SKYBOX,
	PERF_UPDATE_TERRAIN,
	PERF_TIMER_COUNT
};

class MT_Profiler
{
private:
	MT_StatsWindow *m_statsWindow;
	ULONGLONG m_ticksAtLastUpdate;
	UINT m_currentFPS;
	UINT m_frameCountInCurrentSecond;
	UINT m_numberOfTriangles;
	UINT m_numberOfVertices;
	void Init();
public:
	void SetStatsWindow(MT_StatsWindow *statsWindow);
	UINT GetCurrentFPS();
	UINT GetNumberOfVertices();
	UINT GetNumberOfTriangles();
	void Update();
	void RecordNumberOfTriangles(UINT numOfTriangles);
	void RecordNumberOfVertices(UINT numOfVertices);
	static void ProfBegin(PerfTimerType type);
	static void ProfEnd(PerfTimerType type);
	static ULONGLONG ProfGet(PerfTimerType type);
	static MT_Profiler* shared();
};

#endif