#ifndef MT_HEIGHTMAP_H
#define MT_HEIGHTMAP_H

#include <stdio.h>
#include <Windows.h>

struct HeightMapType {
	UINT x, z;
	FLOAT height;
};

class MT_HeightMap {
private:
	FILE *m_fileHandle;
	BITMAPFILEHEADER m_fileHeader;
	BITMAPINFOHEADER m_bitmapHeader;
	UCHAR *m_imageData;
	HeightMapType *m_heightMap;
	UINT m_imageWidth, m_imageHeight;
public:
	MT_HeightMap();
	bool LoadImageData(const char *fileName);
	bool LoadHeightMap();
	void CleanImageData();
	//void Log();
	bool Init(const char *fileName);
	UINT width();
	UINT height();
	FLOAT heightAt(UINT x, UINT z);
	void Clean();
};

#endif