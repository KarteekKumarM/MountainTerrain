#ifndef MT_LOGGER_H
#define MT_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

class MT_Logger
{
public:
	static void Log(char *format, ...);
	static void LogWarning(char *format, ...);
	static void LogError(char *format, ...);
};

#endif