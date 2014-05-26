#ifndef UTILITY_H
#define UTILITY_H

__inline double LinearInterpolate(double a, double b, double x);
__inline double CosineInterpolate(double a, double b, double x);
__inline double Noise(int x);
__inline double SmoothNoise(int x, int y);
__inline double Noise(int x, int y);

void BitMapWrite(uint w, uint h, double** red, double **green, double **blue, char* imagePath);

void InitValues(double ***values, uint x, uint y);
void FreeValues(double ***values, uint x, uint y);
void CopyValues(double **dest, double **src, uint x, uint y);
void AddValues(double **dest, double **src, uint x, uint y);
void MulValues(double **dest, uint scalar, uint x, uint y);
void DivValues(double **dest, uint scalar, uint x, uint y);

#endif