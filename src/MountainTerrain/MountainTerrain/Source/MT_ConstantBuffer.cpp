#include "MT_ConstantBuffer.h"

void MT_ConstantBuffer::InitMatrices(UINT screenWidth, UINT screenHeight) {
	// world matrix
	m_World = DirectX::XMMatrixIdentity();

	// view matrix
	XMVECTOR eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	XMVECTOR at = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_View = DirectX::XMMatrixLookAtLH( eye, at, up );

    // projection matrix
	m_Projection = DirectX::XMMatrixPerspectiveFovLH( XM_PIDIV2, screenWidth/(FLOAT)screenHeight, 0.01f, 100.0f );
}

void MT_ConstantBuffer::InitConstantBuffer(ID3D11Device *d3dDevice) {
	D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory( &constantBufferDesc, sizeof(constantBufferDesc) );
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    constantBufferDesc.ByteWidth = sizeof(ConstantBufferMatrices);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = 0;
    HRESULT hr = d3dDevice->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer);
	if(FAILED(hr)) {
		MessageBox(nullptr, L"Unable to create constant buffer", L"Error", MB_OK);
	}
}

void MT_ConstantBuffer::UpdateConstantBuffer(ID3D11DeviceContext *d3dDeviceContext) {

	// need temp variables on stack to get past bug : 
	// http://www.gamedev.net/topic/627033-xmmatrixtranspose-crashes/

	XMMATRIX world = m_World;
	XMMATRIX view = m_View;
	XMMATRIX projection = m_Projection;

	ConstantBufferMatrices constantBuff;
	constantBuff.mWorld = DirectX::XMMatrixTranspose(world);
	constantBuff.mView = DirectX::XMMatrixTranspose(view);
	constantBuff.mProjection = DirectX::XMMatrixTranspose(projection);
	d3dDeviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuff, 0, 0);
}

void MT_ConstantBuffer::Init(ID3D11Device *d3dDevice, UINT screenWidth, UINT screenHeight) {
	InitMatrices(screenWidth, screenHeight);
	InitConstantBuffer(d3dDevice);
}

void MT_ConstantBuffer::Update(ID3D11DeviceContext *d3dDeviceContext) {
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	UpdateConstantBuffer(d3dDeviceContext);
}

void MT_ConstantBuffer::Clean() {
	if(m_constantBuffer) m_constantBuffer->Release();
}