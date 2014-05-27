#include "MT_HeightMap.h"
#include "MT_Logger.h"

bool MT_HeightMap::LoadHeightMap() 
{
	const FLOAT DIVE_ALL_HEIGHTS_BY = 5.0f;

	bool successFlag;
	m_heightMap = new XMFLOAT3[width() * height()];
	if(m_heightMap) 
	{
		successFlag = true;

		int indexIntoImageData = 0;
		for(UINT z = 0; z < height(); z++) 
		{
			for(UINT x = 0; x < width(); x++ )
			{
				// read height from image data
				UCHAR height = imageReader->dataAtIndex(indexIntoImageData);
				indexIntoImageData += 3;

				// store into height map
				int indexIntoHeightMap = (width() * z)  + x;
				m_heightMap[indexIntoHeightMap].y = height / DIVE_ALL_HEIGHTS_BY;
				m_heightMap[indexIntoHeightMap].x = (FLOAT)x;
				m_heightMap[indexIntoHeightMap].z = (FLOAT)z;
			}
		}
	}
	else 
	{
		successFlag = false;
	}
	return successFlag;
}

IndicesOfTwoTrianglesThatFormACell MT_HeightMap::getIndiciesOfTheTwoTrianglesThatFormACellAtPoint( UINT i, UINT j ) 
{
	IndicesOfTwoTrianglesThatFormACell result;
	UINT i_x_width = i * width();
	UINT i_minus_1_x_width = i_x_width - width(); // same as (i-1) * width()
	
	result.indexOf_UR = i_x_width + j;
	result.indexOf_UL = i_x_width +  j - 1;
	result.indexOf_LL = i_minus_1_x_width + j - 1;
	result.indexOf_LR = i_minus_1_x_width + j;
	return result;
}

UINT MT_HeightMap::width()
{
	return imageReader->width();
}

UINT MT_HeightMap::height()
{
	return imageReader->height();
}

UINT MT_HeightMap::indexOf( UINT i, UINT j )
{
	return (i * width()) + j;
}

XMFLOAT3 MT_HeightMap::heightAt(UINT index)
{
	return m_heightMap[index];
}

XMFLOAT3 MT_HeightMap::heightAt(UINT x, UINT z) 
{
	int index = (width() * z)  + x;
	return m_heightMap[index];
}

void MT_HeightMap::Init(const char *fileName) 
{
	bool sucessFlag;

	imageReader = new MT_BitmapReader();
	sucessFlag = imageReader->Init(fileName);
	if(!sucessFlag)
	{
		MT_Logger::LogError("Unable to read image file : %s", fileName);
	}

	sucessFlag = LoadHeightMap();
	if(!sucessFlag)
	{
		MT_Logger::LogError("Unable to load heightmap");
	}

	// clean up image reader
	imageReader->Clean();
}

void MT_HeightMap::Clean() 
{
	delete imageReader;
	imageReader = 0;

	delete[] m_heightMap;
	m_heightMap = 0;
}