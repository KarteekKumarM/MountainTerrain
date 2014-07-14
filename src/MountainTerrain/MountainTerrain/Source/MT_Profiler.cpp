#include "MT_Profiler.h"
#include "MT_Logger.h"
#include "MT_StatsWindow.h"

#include <DirectXMath.h>
using namespace DirectX;

#define TICKS_PER_MILL_SECOND 10000
#define MILLISECONDS_PER_SECOND 1000

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

LARGE_INTEGER  PerfTimerBeginTimes[PERF_TIMER_COUNT];
LARGE_INTEGER  PerfTimerEndTimes[PERF_TIMER_COUNT];

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

static ULONGLONG ProfGet(PerfTimerType type)
{
	assert(type < PERF_TIMER_COUNT);

	ULONGLONG retTime = PerfTimerEndTimes[type].QuadPart - PerfTimerBeginTimes[type].QuadPart;
	return retTime;
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

UINT MT_Profiler::GetCurrentFPS()
{
	ULONGLONG ticks = PerfTimerEndTimes[PERF_RENDER].QuadPart - PerfTimerBeginTimes[PERF_RENDER].QuadPart;
	FLOAT milliSeconds = (FLOAT)ticks / TICKS_PER_MILL_SECOND;
	m_currentFPS = (UINT)MILLISECONDS_PER_SECOND / (milliSeconds);
	return m_currentFPS;
}

UINT MT_Profiler::GetNumberOfVertices()
{
	return m_numberOfVertices;
}

UINT MT_Profiler::GetNumberOfTriangles()
{
	return m_numberOfTriangles;
}

void MT_Profiler::Init()
{
	m_currentFPS = 0;
}

void MT_Profiler::Update()
{
	ULONGLONG ticksSinceLastUpdate = (PerfTimerEndTimes[PERF_RENDER].QuadPart - m_ticksAtLastUpdate);
	if (ticksSinceLastUpdate > ( TICKS_PER_MILL_SECOND * MILLISECONDS_PER_SECOND ) / UPDATES_PER_SECOND)
	{
		m_ticksAtLastUpdate = PerfTimerEndTimes[PERF_RENDER].QuadPart;
		m_statsWindow->Redraw();
	}
}

void MT_Profiler::RecordNumberOfVertices(UINT numOfVertices)
{
	m_numberOfVertices = numOfVertices;
}

void MT_Profiler::RecordNumberOfTriangles(UINT numberOfTriangles)
{
	m_numberOfTriangles = numberOfTriangles;
}