#include <math.h>
#include "Defines.h"

double LinearInterpolate(double a, double b, double x)
{
	return a * (1 - x) + b * x;
}

double CosineInterpolate(double a, double b, double x)
{
	double ft = x * PI;
	double f = (1 - cos(ft)) * 0.5;
	return LinearInterpolate(a, b, f);
}

double CubicInterpolate(double a, double b, double x)
{
	double x2 = x * x;
	double x3 = x2 * x;
	double f = (3 * x2) - (2 * x3);
	return LinearInterpolate(a, b, f);
}

double Noise(int x)
{
	x = (x << 13) ^ x;
	return (double)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double Noise(int x, int y)
{
	int n = x + (y * 57);
	n = (n << 13) ^ n;
	int nn = (n*(n*n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0 - ((double)nn / 1073741824.0);
}

double SmoothNoise(int x, int y)
{
	double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	double center = Noise(x, y) / 4;
	return corners + sides + center;
}

void InitValues(double ***values, uint x, uint y)
{
	*values = new double*[x];
	for (uint i = 0; i < x; i++)
	{
		(*values)[i] = new double[y];
		for (uint j = 0; j < y; j++)
		{
			(*values)[i][j] = 0;
		}
	}
}

void FreeValues(double ***values, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		delete (*values)[i];
	}
	delete (*values);
	values = 0;
}

void CopyValues(double **dest, double **src, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		for (uint j = 0; j < y; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

void AddValues(double **dest, double **src, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		for (uint j = 0; j < y; j++)
		{
			dest[i][j] += src[i][j];
		}
	}
}

void AddValues(double **dest, double scalar, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		for (uint j = 0; j < y; j++)
		{
			dest[i][j] += scalar;
		}
	}
}

void MulValues(double **dest, uint scalar, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		for (uint j = 0; j < y; j++)
		{
			dest[i][j] *= scalar;
		}
	}
}

void DivValues(double **dest, uint scalar, uint x, uint y)
{
	for (uint i = 0; i < x; i++)
	{
		for (uint j = 0; j < y; j++)
		{
			dest[i][j] /= scalar;
		}
	}
}