#include "MT_HeightMap.h"

MT_HeightMap::MT_HeightMap() {
	m_imageWidth = 0;
	m_imageHeight = 0;
}

bool MT_HeightMap::LoadImageData(const char *fileName) {
	int error;
	// open as binary
	error = fopen_s(&m_fileHandle, fileName, "rb");

	if(error == 0) {
		// read file header
		int count = 0;
		count = fread(&m_fileHeader, sizeof(BITMAPFILEHEADER), 1, m_fileHandle);
		if(count == 1) {
			// read bitmap info header
			count = fread(&m_bitmapHeader, sizeof(BITMAPINFOHEADER), 1, m_fileHandle);
			m_imageWidth = m_bitmapHeader.biWidth;
			m_imageHeight = m_bitmapHeader.biHeight;
			if(count == 1) {
				LONG imageDataSize = m_imageHeight * m_imageWidth * 3;
				// allocate memory for the image data
				m_imageData = new UCHAR[imageDataSize];
				if(m_imageData) {
					// seek to start of bitmap data
					fseek(m_fileHandle, m_fileHeader.bfOffBits, SEEK_SET);
					// read image data
					count = fread(m_imageData, 1, imageDataSize, m_fileHandle);
					if(count == imageDataSize) {
						// close the file
						error = fclose(m_fileHandle);
					} else {
						error = 1;
					}
				} else {
					error = 1;
				}
			}
		}
	}
	return error == 0;
}

bool MT_HeightMap::LoadHeightMap() {
	bool successFlag;
	m_heightMap = new HeightMapType[m_imageWidth * m_imageHeight];
	if(m_heightMap) {
		successFlag = true;

		int indexIntoImageData = 0;
		for(int z = 0; z < m_imageHeight; z++) {
			for(int x = 0; x < m_imageWidth; x++ ){
				// read height from image data
				UCHAR height = m_imageData[indexIntoImageData]; 
				indexIntoImageData += 1;

				// store into height map
				int indexIntoHeightMap = (m_imageWidth * z)  + x;
				m_heightMap[indexIntoHeightMap].height = (FLOAT)height;
				m_heightMap[indexIntoHeightMap].x = x;
				m_heightMap[indexIntoHeightMap].z = z;
			}
		}

	} else {
		successFlag = false;
	}
	return successFlag;
}

void MT_HeightMap::CleanImageData() {
	delete[] m_imageData;
	m_imageData = 0;
}

UINT MT_HeightMap::width() {
	return m_imageWidth;
}

UINT MT_HeightMap::height() {
	return m_imageHeight;
}

FLOAT MT_HeightMap::heightAt(UINT x, UINT z) {
	return (m_imageWidth * z)  + x;
}

/*void MT_HeightMap::Log() {
	for(int i = 0; i < (m_imageWidth * m_imageHeight); i++) {
		char str[256];
		sprintf_s(str, sizeof(str), "%u,%u--%f\n", m_heightMap[i].x, m_heightMap[i].z, m_heightMap[i].height);
		OutputDebugStringA(str);
	}
}*/

bool MT_HeightMap::Init(const char *fileName) {
	bool sucessFlag= LoadImageData(fileName);
	if(sucessFlag) {
		sucessFlag = LoadHeightMap();
		CleanImageData();
	}
	return sucessFlag;
}

void MT_HeightMap::Clean() {
	delete[] m_heightMap;
	m_heightMap = 0;
}