#ifndef MT_TEXTURE_H
#define MT_TEXTURE_H

#include <d3d11.h>

class MT_Texture
{
private:
	ID3D11Texture2D *m_grassTexture;
	ID3D11Texture2D *m_rockTexture;
	ID3D11Texture2D *m_waterTexture;
	ID3D11ShaderResourceView *m_grassTexShaderResourceView;
	ID3D11ShaderResourceView *m_rockTexShaderResourceView;
	ID3D11ShaderResourceView *m_waterTexShaderResourceView;
	ID3D11SamplerState *m_samplerState;
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* grasstextureFileName, char *rockTextureFileName, char *waterTextureFileName);
	void Clean();
};

#endif