#include "MT_Utility.h"

#include "MT_Logger.h"

#include <ctime>
#include <stdio.h>

void GetTimeString(char **timeResult)
{
	const int TIME_STRING_SIZE = 20;
	char *timeStringBuffer = new char[TIME_STRING_SIZE];
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	sprintf_s(timeStringBuffer, TIME_STRING_SIZE, "%d-%02i-%02i %02i.%02i.%02i", (now.tm_year + 1900), (now.tm_mon + 1), now.tm_mday, 
																				  now.tm_hour, now.tm_min, now.tm_sec);
	*timeResult = timeStringBuffer;
}

XMFLOAT3 XMFloat3Subtract(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

XMFLOAT3 XMFloat3Cross(XMFLOAT3 a, XMFLOAT3 b)
{
	XMFLOAT3 result;
	result.x = (a.y * b.z) - (b.y*a.z);
	result.y = -1 * ((a.x * b.z) - (b.x*a.z));
	result.z = (a.x * b.y) - (b.x*a.z);
	return result;
}

XMFLOAT3 XMFloat3Multiply(FLOAT scalar, XMFLOAT3 vector)
{
	return XMFLOAT3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

XMFLOAT3 XMFloat3Divide(FLOAT scalar, XMFLOAT3 vector)
{
	return XMFLOAT3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}

XMFLOAT3 XMFloat3Add(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

XMFLOAT3 XMFloat3Average(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFloat3Multiply(0.5f, XMFloat3Add(a, b));
}