#ifndef MT_TEXTURE_H
#define MT_TEXTURE_H

#include <d3d11.h>

class MT_Texture
{
private:
	ID3D11Texture2D *m_texture;
	ID3D11ShaderResourceView *m_shaderResourceView;
	ID3D11SamplerState *m_samplerState;
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* textureFileName);
	void Clean();
};

#endif