#ifndef MT_HEIGHTMAP_H
#define MT_HEIGHTMAP_H

#include <stdio.h>
#include <Windows.h>

struct HeightMapType 
{
	UINT x, z;
	FLOAT height;
};

struct IndicesOfTwoTrianglesThatFormACell 
{
	WORD indexOf_UR;
	WORD indexOf_UL;
	WORD indexOf_LL;
	WORD indexOf_LR;
};

class MT_HeightMap 
{
private:
	FILE *m_fileHandle;
	BITMAPFILEHEADER m_fileHeader;
	BITMAPINFOHEADER m_bitmapHeader;
	UCHAR *m_imageData;
	HeightMapType *m_heightMap;
	UINT m_imageWidth, m_imageHeight;
	bool LoadImageData(const char *fileName);
	bool LoadHeightMap();
	void CleanImageData();
public:
	MT_HeightMap();
	bool Init(const char *fileName);
	UINT width();
	UINT height();
	IndicesOfTwoTrianglesThatFormACell getIndiciesOfTheTwoTrianglesThatFormACellAtPoint( UINT i, UINT j );
	FLOAT heightAt(UINT x, UINT z);
	HeightMapType heightMapStructAt(UINT index);
	void Clean();
};

#endif