#ifndef MT_LIGHT_H
#define MT_LIGHT_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

struct LightBufferValues
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 direction;
	BOOL light_enabled;
	BOOL texture_enabled;
	
	// extra unused variables - size must be mulitple of 16
	BOOL extra, extraa, extraaa;
};

class MT_Light
{
private:
	ID3D11Buffer *m_lightBuffer;
	D3D11_MAPPED_SUBRESOURCE m_lightBuffer_mappedResource;
	LightBufferValues m_lightBufferValues;
	ID3D11DeviceContext *m_d3dDeviceContext;
	void UpdateLightBuffer();
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, LightBufferValues lightBufferValues);
	void Toggle(bool shouldToggleLight, bool shouldToggleTexture);
	void Clean();
};
#endif