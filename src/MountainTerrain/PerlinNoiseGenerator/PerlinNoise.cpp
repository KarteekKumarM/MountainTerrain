#include "Defines.h"
#include "Utility.h"

struct Lattice
{
	uint x, y;
	uint numOfSamplePoints;
	uint numOfOctaves;
	double **samples;
};

void LatticeInit(Lattice *lattice, uint x, uint y, uint numOfSamplePoints, uint octaves)
{
	lattice->x = x;
	lattice->y = y;
	lattice->numOfSamplePoints = numOfSamplePoints;
	lattice->numOfOctaves = octaves;
	InitValues(&lattice->samples, x * numOfSamplePoints, y * numOfSamplePoints);
}

double InterpolatedNoiseAt(double x, double y)
{
	int intX = (int)x;
	double fracX = x - intX;

	int intY = (int)y;
	double fracY = y - intY;

	double v1 = SmoothNoise(intX, intY);
	double v2 = SmoothNoise(intX + 1, intY);
	double v3 = SmoothNoise(intX, intY + 1);
	double v4 = SmoothNoise(intX + 1, intY + 1);

	double i1 = CubicInterpolate(v1, v2, fracX);
	double i2 = CubicInterpolate(v3, v4, fracX);

	double f = CubicInterpolate(i1, i2, fracY);

	return f;
}

double LatticeSampleAt(Lattice *lattice, double x, double y)
{
	double value = 0.0;
	double persistance = 1.0 / 4.0;

	double freq = 1.0;
	double amplitude = persistance;
	for (uint o = 0; o < lattice->numOfOctaves; o++)
	{
		value += InterpolatedNoiseAt(x * freq, y * freq) * amplitude;
		freq *= 2.0;
		amplitude *= persistance;
	}
	return value;
}

void LatticeSample(Lattice *lattice)
{
	uint xx = (lattice->x - 1) * lattice->numOfSamplePoints;
	uint yy = (lattice->y - 1) * lattice->numOfSamplePoints;
	for (uint i = 0; i < xx; i++)
	{
		for (uint j = 0; j < yy; j++)
		{
			// sample all points
			double dI = 1.0 * i / lattice->numOfSamplePoints;
			double dJ = 1.0 * j / lattice->numOfSamplePoints;
			lattice->samples[i][j] = LatticeSampleAt(lattice, dI, dJ);
		}
	}
}

void LatticeFree(Lattice *lattice)
{
	FreeValues(&lattice->samples, lattice->x & lattice->numOfSamplePoints, lattice->y & lattice->numOfSamplePoints);
}

// ---------------- Perlin algorithm
// determine lattice and seed with random gradient vectors
// Pick sample point in the region
// Calculate teh displacement of the corners fof the lattice relative to the sample point
// Calculate the gradient valkues at the sample point by taking the dot product of the displacement vectors and their respective gradeint vectors
// Interpolate the four gradient values at the sample point
void PerlinNoise( double ***values, uint width, uint height, uint octaves )
{
	InitValues(values, width, height);
	Lattice *lattice = new Lattice();;
	LatticeInit(lattice, width / 8, height / 8, 8, octaves);
	LatticeSample(lattice);
	AddValues(*values, lattice->samples, width, height);
	LatticeFree(lattice);
	delete lattice;
}
