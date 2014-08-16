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

enum PerfStatType
{
	PERF_TRIANGLE_COUNT,
	PERF_VERTEX_COUNT,
	PERF_VERTEX_BUFFER_SIZE,
	PERF_INDEX_BUFFER_SIZE,

	PERF_STAT_COUNT
};

class MT_Profiler
{
private:
	MT_StatsWindow *m_statsWindow;
	ULONGLONG m_ticksAtLastUpdate;
	LARGE_INTEGER m_freq;
	UINT m_currentFPS;
	UINT m_frameCountInCurrentSecond;
	void Init();
public:
	void SetStatsWindow(MT_StatsWindow *statsWindow);
	float GetCurrentFPS();
	void Update();
	static void ProfRecordStat(PerfStatType type, float value);
	static void ProfBegin(PerfTimerType type);
	static void ProfEnd(PerfTimerType type);
	float ProfGetTimerValue(PerfTimerType type);
	float ProfGetStatValue(PerfStatType type);
	static std::string ProfGetTimerName(PerfTimerType type);
	static std::string ProfGetStatName(PerfStatType type);
	static MT_Profiler* shared();
	void static Wait(FLOAT seconds);
};

#endif
