#ifndef MT_PROFILER_H
#define MT_PROFILER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class MT_Profiler
{
private:
	ULONGLONG m_timeAtLastSecond;
	ULONGLONG m_startTime;
	ULONGLONG m_totalFrames;
	ULONGLONG m_totalSeconds;
	ULONGLONG m_currentFPS;
	ULONGLONG m_frameCountInCurrentSecond;
public:
	ULONGLONG GetAverageFPS();
	ULONGLONG GetCurrentFPS();
	void Init();
	void Frame();
	void Display(ID3D11Device *m_d3dDevice, ID3D11DeviceContext *m_d3dDeviceContext);
};

#endif