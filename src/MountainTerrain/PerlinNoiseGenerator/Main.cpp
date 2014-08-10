#include <Windows.h>
#include "PerlinNoise.h"
#include "Defines.h"
#include "Utility.h"

// Entry point for Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	)
{
	const uint width = 2048 * 2;
	const uint height = 2048 * 2;

	double **values = 0;
	PerlinNoise(&values, width, height, 8);
	BitMapWrite(width, height, values, values, values, HEIGHT_MAP_IMAGE_PATH);

	return 0;
}
