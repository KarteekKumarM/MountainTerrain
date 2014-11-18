#ifndef MT_BITMAPREADER_H
#define MT_BITMAPREADER_H

#include <Windows.h>
#include <stdio.h>

class MT_BitmapReader
{
private:
	FILE *m_fileHandle;
	BITMAPFILEHEADER m_fileHeader;
	BITMAPINFOHEADER m_bitmapHeader;
	UCHAR *m_imageData;
public:
	bool Init(const char *fileName);
	LONG width();
	LONG height();
	UCHAR* data();
	UCHAR dataAtIndex(UINT index);
	void Clean();
};

#endif