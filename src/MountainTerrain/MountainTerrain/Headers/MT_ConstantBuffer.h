#ifndef MT_CONSTANT_BUFFER_H
#define MT_CONSTANT_BUFFER_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_Camera.h"

struct ConstantBufferMatrices {
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class MT_ConstantBuffer {
private:
	ID3D11Buffer *m_constantBuffer;

	XMMATRIX m_World;
	XMMATRIX m_Projection;

	void InitMatrices(UINT screenWidth, UINT screenHeight);
	void InitConstantBuffer(ID3D11Device *d3dDevice);
	void UpdateConstantBuffer(ID3D11DeviceContext *d3dDeviceContext, MT_Camera *camera);
public:
	void Init(ID3D11Device *d3dDevice, UINT screenWidth, UINT screenHeight);
	void Update(ID3D11DeviceContext *d3dDeviceContext, MT_Camera *camera);
	void Clean();
};

#endif