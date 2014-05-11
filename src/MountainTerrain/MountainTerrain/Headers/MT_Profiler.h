#ifndef MT_PROFILER_H
#define MT_PROFILER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <string>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class MT_StatsWindow;

//enum ProfiledStatsTypes
//{
//	CURRENT_FPS,
//	AVERAGE_FPS,
//	NUMBER_OF_VERTICES,
//	NUMBER_OF_TRIANGLES,
//
//	PROFILED_STATS_COUNT
//};
//
//std::string ProfiledStatsNames[]
//{
//	"Current FPS",
//	"Average FPS",
//	"Number of Vertices",
//	"Number of Triangles"
//};
//
//FLOAT ProfiledStatsValues[]
//{
//		0,
//		0,
//		0,
//		0
//};

class MT_Profiler
{
private:
	MT_StatsWindow *m_statsWindow;
	ULONGLONG m_timeAtLastSecond;
	ULONGLONG m_startTime;
	ULONGLONG m_totalFrames;
	UINT m_currentFPS;
	UINT m_frameCountInCurrentSecond;
	UINT m_numberOfTriangles;
	UINT m_numberOfVertices;
	void Init();
public:
	void SetStatsWindow(MT_StatsWindow *statsWindow);
	FLOAT GetAverageFPS();
	UINT GetCurrentFPS();
	UINT GetNumberOfVertices();
	UINT GetNumberOfTriangles();
	void RecordNumberOfTriangles(UINT numOfTriangles);
	void RecordNumberOfVertices(UINT numOfVertices);
	void RecordFrame();
	static MT_Profiler* shared();
};

#endif