#include "MT_Profiler.h"
#include "MT_Logger.h"
#include "MT_StatsWindow.h"

#include <DirectXMath.h>
using namespace DirectX;

#define MILLISECONDS_PER_SECOND 1000
#define TICKS_PER_MILL_SECOND 1000
#define TICKS_PER_SECOND ( TICKS_PER_MILL_SECOND * MILLISECONDS_PER_SECOND )

#define UPDATES_PER_SECOND 5

static MT_Profiler* sharedInstance;

std::string PerfTimerNames[]
{
	"Update and Render Frame",
	"Render Skybox",
	"Render Terrain",
	"Update Skybox",
	"Update Terrain",
};

std::string PerfStatNames[]
{
	"Triangle Count",
	"Vertex Count",
	"Vertex Buffer Size",
	"Index Buffer Size"
};

LARGE_INTEGER  PerfTimerBeginTimes[PERF_TIMER_COUNT];
LARGE_INTEGER  PerfTimerEndTimes[PERF_TIMER_COUNT];
float PerfStatValue[PERF_STAT_COUNT];

void MT_Profiler::ProfBegin(PerfTimerType type)
{
	assert(type < PERF_TIMER_COUNT);

	LARGE_INTEGER timer;
	QueryPerformanceCounter(&timer);

	PerfTimerBeginTimes[type] = timer;
}

void MT_Profiler::ProfEnd(PerfTimerType type)
{
	assert(type < PERF_TIMER_COUNT);

	LARGE_INTEGER timer;
	QueryPerformanceCounter(&timer);

	PerfTimerEndTimes[type] = timer;
}

std::string MT_Profiler::ProfGetTimerName(PerfTimerType type)
{
	assert(type < PERF_TIMER_COUNT);
	return PerfTimerNames[type];
}

std::string MT_Profiler::ProfGetStatName(PerfStatType type)
{
	assert(type < PERF_STAT_COUNT);
	return PerfStatNames[type];
}

float MT_Profiler::ProfGetTimerValue(PerfTimerType type)
{
	assert(type < PERF_TIMER_COUNT);
	ULONGLONG retTime = PerfTimerEndTimes[type].QuadPart - PerfTimerBeginTimes[type].QuadPart;
	return 1000.0 * retTime / m_freq.QuadPart;
}

float MT_Profiler::ProfGetStatValue(PerfStatType type)
{
	assert(type < PERF_STAT_COUNT);
	return PerfStatValue[type];
}

void MT_Profiler::SetStatsWindow(MT_StatsWindow *statsWindow)
{
	m_statsWindow = statsWindow;
}

MT_Profiler* MT_Profiler::shared()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new MT_Profiler();
		sharedInstance->Init();
	}
	return sharedInstance;
}

float MT_Profiler::GetCurrentFPS()
{
	ULONGLONG ticks = PerfTimerEndTimes[PERF_RENDER].QuadPart - PerfTimerBeginTimes[PERF_RENDER].QuadPart;
	float secondsToRenderOneFrame = 1.0 * ticks / m_freq.QuadPart;
	return 1.0 / secondsToRenderOneFrame;
}

void MT_Profiler::Init()
{
	m_currentFPS = 0;
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_freq);
}

void MT_Profiler::Wait(FLOAT seconds)
{
	LARGE_INTEGER startTimer;
	QueryPerformanceCounter(&startTimer);

	LARGE_INTEGER timer;
	QueryPerformanceCounter(&timer);

	while ((timer.QuadPart - startTimer.QuadPart) < TICKS_PER_SECOND * seconds)
	{
		ULONGLONG tim = timer.QuadPart - startTimer.QuadPart;
		QueryPerformanceCounter(&timer);
	}
}

void MT_Profiler::Update()
{
	ULONGLONG ticksSinceLastUpdate = (PerfTimerEndTimes[PERF_RENDER].QuadPart - m_ticksAtLastUpdate);

	if (ticksSinceLastUpdate > TICKS_PER_SECOND / UPDATES_PER_SECOND)
	{
		m_ticksAtLastUpdate = PerfTimerEndTimes[PERF_RENDER].QuadPart;
		m_statsWindow->Redraw();
	}
}

void MT_Profiler::ProfRecordStat(PerfStatType type, float value)
{
	assert(type < PERF_STAT_COUNT);
	PerfStatValue[type] = value;
}