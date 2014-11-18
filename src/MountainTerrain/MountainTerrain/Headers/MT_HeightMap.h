#ifndef MT_HEIGHTMAP_H
#define MT_HEIGHTMAP_H

#include <stdio.h>
#include <Windows.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_BitmapReader.h"

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
	XMFLOAT3 *m_heightMap;
	bool LoadHeightMap();
	MT_BitmapReader *imageReader;
public:
	void Init(const char *fileName);
	UINT indexOf( UINT i, UINT j );
	UINT width();
	UINT height();
	IndicesOfTwoTrianglesThatFormACell getIndiciesOfTheTwoTrianglesThatFormACellAtPoint( UINT i, UINT j );
	XMFLOAT3 heightAt(UINT x, UINT z);
	XMFLOAT3 heightAt(UINT index);
	void Clean();
};

#endif