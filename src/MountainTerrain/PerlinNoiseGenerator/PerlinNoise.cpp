#include "Defines.h"
#include "Utility.h"

struct Lattice
{
	uint x, y;
	uint numOfSamplePoints;
	double **values;
	double **samples;
};

void LatticeInit(Lattice *lattice, uint x, uint y, uint numOfSamplePoints)
{
	lattice->x = x;
	lattice->y = y;
	lattice->numOfSamplePoints = numOfSamplePoints;
	InitValues(&lattice->values, x, y);
	InitValues(&lattice->samples, x * numOfSamplePoints, y * numOfSamplePoints);
}

void LatticeSeed(Lattice *lattice)
{
	for (uint i = 0; i < lattice->x; i++)
	{
		for (uint j = 0; j < lattice->y; j++)
		{
			lattice->values[i][j] = SmoothNoise(i, j);
		}
	}
}

void LatticeSample(Lattice *lattice)
{
	uint xx = (lattice->x - 1) * lattice->numOfSamplePoints;
	uint yy = (lattice->y - 1) * lattice->numOfSamplePoints;
	for (uint i = 0; i < xx; i++)
	{
		uint vX = i / lattice->numOfSamplePoints;
		for (uint j = 0; j < yy; j++)
		{
			uint vY = j / lattice->numOfSamplePoints;

			double s = lattice->values[vX][vY];
			double t = lattice->values[vX + 1][vY];
			double u = lattice->values[vX][vY + 1];
			double v = lattice->values[vX + 1][vY + 1];

			double lerpX = i - (vX * lattice->numOfSamplePoints);
			double lerpY = j - (vY * lattice->numOfSamplePoints);
			double st = CosineInterpolate(s, t, lerpX); 
			double uv = CosineInterpolate(u, v, lerpX);

			lattice->samples[i][j] = CosineInterpolate(st, uv, lerpY);
		}
	}
}

void LatticeFree(Lattice *lattice)
{
	FreeValues(&lattice->values, lattice->x, lattice->y);
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

	uint frequeny = 1;
	while (octaves > 0)
	{
		Lattice *lattice = new Lattice();;
		LatticeInit(lattice, width, height, frequeny);
		LatticeSeed(lattice);
		LatticeSample(lattice);
		AddValues(*values, lattice->values, width, height);
		LatticeFree(lattice);
		delete lattice;

		octaves--;
		frequeny *= 2;
	}
	
	//DivValues(*values, octaves, width, height);
}
