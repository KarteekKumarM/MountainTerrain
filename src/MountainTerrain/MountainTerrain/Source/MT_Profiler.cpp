#include "MT_Profiler.h"
#include "MT_Logger.h"
#include "MT_StatsWindow.h"

#define MILLISECONDS_IN_SECOND 1000

static MT_Profiler* sharedInstance;

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

FLOAT MT_Profiler::GetAverageFPS()
{
	ULONGLONG timeSinceStart = GetTickCount64() - m_startTime;
	FLOAT average = timeSinceStart > 0 ? (FLOAT)(MILLISECONDS_IN_SECOND * m_totalFrames / timeSinceStart) : 0;
	return average;
}

UINT MT_Profiler::GetCurrentFPS()
{
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
	m_totalFrames = 0;
	m_frameCountInCurrentSecond = 0;
	m_startTime = GetTickCount64();
	m_timeAtLastSecond = m_startTime;
}

void MT_Profiler::RecordNumberOfVertices(UINT numOfVertices)
{
	m_numberOfVertices = numOfVertices;
}

void MT_Profiler::RecordNumberOfTriangles(UINT numberOfTriangles)
{
	m_numberOfTriangles = numberOfTriangles;
}

void MT_Profiler::RecordFrame()
{
	m_totalFrames++;
	m_frameCountInCurrentSecond++;
	ULONGLONG currentTime = GetTickCount64();
	ULONGLONG timeDiff = currentTime - m_timeAtLastSecond;
	if (timeDiff >= MILLISECONDS_IN_SECOND)
	{
		m_currentFPS = m_frameCountInCurrentSecond;

		// reset
		m_frameCountInCurrentSecond = 0;
		m_timeAtLastSecond = currentTime;

		// redraw stats every second
		if (m_statsWindow != NULL)
		{
			m_statsWindow->Redraw();
		}
	}
}