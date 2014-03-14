#ifndef MT_HEIGHTMAP_H
#define MT_HEIGHTMAP_H

#include <stdio.h>
#include <Windows.h>

#include <DirectXMath.h>
using namespace DirectX;

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
	XMFLOAT3 *m_heightMap;
	UINT m_imageWidth, m_imageHeight;
	bool LoadImageData(const char *fileName);
	bool LoadHeightMap();
	void CleanImageData();
public:
	MT_HeightMap();
	bool Init(const char *fileName);
	UINT indexOf( UINT i, UINT j );
	UINT width();
	UINT height();
	IndicesOfTwoTrianglesThatFormACell getIndiciesOfTheTwoTrianglesThatFormACellAtPoint( UINT i, UINT j );
	XMFLOAT3 heightAt(UINT x, UINT z);
	XMFLOAT3 heightAt(UINT index);
	void Clean();
};

#endif