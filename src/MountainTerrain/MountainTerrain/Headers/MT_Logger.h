#ifndef MT_LOGGER_H
#define MT_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

class MT_Logger
{
private:
	static MT_Logger *sharedInstance;
public:
	static void Log( char *format, ... )
	{
		va_list args;
		va_start(args, format);

		char buffer[512];
		vsprintf_s (buffer, sizeof(buffer), format, args);
		OutputDebugStringA(buffer);

		va_end(args);
	}

	static void LogWarning( char *format, ... )
	{
		va_list args;
		va_start(args, format);

		char buffer[512];
		vsprintf_s (buffer, sizeof(buffer), format, args);
		OutputDebugStringA(buffer);

		va_end(args);
	}

	static void LogError( char *format, ... )
	{
		va_list args;
		va_start(args, format);

		char buffer[512];
		vsprintf_s (buffer, sizeof(buffer), format, args);
		OutputDebugStringA(buffer);
		MessageBoxA(nullptr, buffer, "Error", MB_OK);

		va_end(args);
	}
};

#endif