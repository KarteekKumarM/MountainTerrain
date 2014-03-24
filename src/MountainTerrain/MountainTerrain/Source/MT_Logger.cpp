#include "MT_Logger.h"

void MT_Logger::Log(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	OutputDebugStringA(buffer);

	va_end(args);
}

void MT_Logger::LogWarning(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	OutputDebugStringA(buffer);

	va_end(args);
}

void MT_Logger::LogError(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[512];
	vsprintf_s(buffer, sizeof(buffer), format, args);
	OutputDebugStringA(buffer);
	MessageBoxA(nullptr, buffer, "Error", MB_OK);

	va_end(args);

	PostQuitMessage(0);
}