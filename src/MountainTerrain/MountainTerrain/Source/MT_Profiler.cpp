#include "MT_Profiler.h"
#include "MT_Logger.h"

#define MILLISECONDS_IN_SECOND 1000

ULONGLONG MT_Profiler::GetAverageFPS()
{
	ULONGLONG average = (ULONGLONG)(m_totalFrames / m_totalSeconds);
	return average;
}

ULONGLONG MT_Profiler::GetCurrentFPS()
{
	return m_currentFPS;
}

void MT_Profiler::Init()
{
	m_currentFPS = 0;
	m_totalFrames = 0;
	m_frameCountInCurrentSecond = 0;
	m_totalSeconds = 0;
	m_startTime = GetTickCount64();
	m_timeAtLastSecond = m_startTime;
}

void MT_Profiler::Frame()
{
	m_totalFrames++;
	m_frameCountInCurrentSecond++;
	ULONGLONG currentTime = GetTickCount64();
	if (m_timeAtLastSecond - currentTime >= MILLISECONDS_IN_SECOND)
	{
		m_currentFPS = m_frameCountInCurrentSecond;
		m_totalSeconds++;

		// reset
		m_frameCountInCurrentSecond = 0;
		m_timeAtLastSecond = currentTime;
	}
}

void MT_Profiler::Display(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext)
{
	//MT_Logger::Log("Current FPS : %d\n", GetCurrentFPS());
	//MT_Logger::Log("Average FPS : %d\n", GetAverageFPS());
}