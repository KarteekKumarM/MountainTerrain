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
	// to make it a multiple of 16
	float padding;
};

class MT_Light
{
private:
	ID3D11Buffer *m_lightBuffer;
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, LightBufferValues lightBufferValues);
	void Clean();
};
#endif