#ifndef MT_TEXTURE_H
#define MT_TEXTURE_H

#include <d3d11.h>
#include <vector>

class MT_Texture
{
private:
	std::vector<ID3D11Texture2D*> m_textures;
	std::vector<ID3D11ShaderResourceView*> m_shaderResourceViews;
	ID3D11SamplerState *m_samplerState;
public:
	void LoadSampler(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadTexture(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* textureFileName);
	void SetShaderResources(ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
};

#endif