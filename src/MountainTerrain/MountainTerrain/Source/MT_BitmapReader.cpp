#include "MT_BitmapReader.h"
#include "MT_Logger.h"

void MT_BitmapReader::Clean()
{
	delete[] m_imageData;
	m_imageData = 0;
}

LONG MT_BitmapReader::width()
{
	return m_bitmapHeader.biWidth;
}

LONG MT_BitmapReader::height()
{
	return m_bitmapHeader.biHeight;
}

UCHAR MT_BitmapReader::dataAtIndex(UINT index)
{
	return m_imageData[index];
}

UCHAR* MT_BitmapReader::data()
{
	return m_imageData;
}

bool MT_BitmapReader::Init(const char *fileName)
{
	int error;
	// open as binary
	error = fopen_s(&m_fileHandle, fileName, "rb");

	if (error == 0 && m_fileHandle != 0) {
		// read file header
		int count = 0;
		count = fread(&m_fileHeader, sizeof(BITMAPFILEHEADER), 1, m_fileHandle);
		if(count == 1)
		{
			// read bitmap info header
			count = fread(&m_bitmapHeader, sizeof(BITMAPINFOHEADER), 1, m_fileHandle);
			if(count == 1) 
			{
				LONG imageDataSize = m_bitmapHeader.biHeight * m_bitmapHeader.biWidth * 3;
				// allocate memory for the image data
				m_imageData = new UCHAR[imageDataSize];
				if(m_imageData)
				{
					// seek to start of bitmap data
					fseek(m_fileHandle, m_fileHeader.bfOffBits, SEEK_SET);
					// read image data
					count = fread(m_imageData, 1, imageDataSize, m_fileHandle);
					if(count == imageDataSize) 
					{
						// close the file
						error = fclose(m_fileHandle);
					} 
					else
					{
						error = 1;
					}
				} 
				else
				{
					error = 1;
				}
			}
		}
	}
	else
	{
		MT_Logger::LogError("Unable to open image file : %s", fileName);
	}
	return error == 0;
}