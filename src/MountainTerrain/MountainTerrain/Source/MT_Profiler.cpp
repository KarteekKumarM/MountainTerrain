#include "MT_Profiler.h"
#include "MT_Logger.h"

/*#include "SpriteFont.h"
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;*/

#define MILLISECONDS_IN_SECOND 1000

static MT_Profiler* sharedInstance;

MT_Profiler* MT_Profiler::shared()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new MT_Profiler();
		sharedInstance->Init();
	}
	return sharedInstance;
}

ULONGLONG MT_Profiler::GetAverageFPS()
{
	//ULONGLONG average = (ULONGLONG)(m_totalFrames / m_totalSeconds);
	return 0;
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

void MT_Profiler::RecordFrame()
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