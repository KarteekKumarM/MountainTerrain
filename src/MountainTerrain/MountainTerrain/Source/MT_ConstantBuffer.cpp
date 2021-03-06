#include "MT_ConstantBuffer.h"

void MT_ConstantBuffer::InitMatrices(UINT screenWidth, UINT screenHeight)
{
	// world matrix
	m_World = DirectX::XMMatrixIdentity();

    // projection matrix
	m_Projection = DirectX::XMMatrixPerspectiveFovLH( XM_PI  * 0.4, (FLOAT)screenWidth/(FLOAT)screenHeight, 0.1f, 1000.0f );
}

void MT_ConstantBuffer::InitConstantBuffer(ID3D11Device *d3dDevice)
{
	D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory( &constantBufferDesc, sizeof(constantBufferDesc) );
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.ByteWidth = sizeof(ConstantBufferValues);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = 0;
    HRESULT hr = d3dDevice->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer);
	if(FAILED(hr)) {
		MessageBox(nullptr, L"Unable to create constant buffer", L"Error", MB_OK);
	}
}

void MT_ConstantBuffer::UpdateConstantBuffer(ID3D11DeviceContext *d3dDeviceContext, MT_Camera *camera) 
{

    // Temp code : Rotate the cube by rotating the world
    static float t = 0.0f;
	static ULONGLONG timeStart = 0;
    ULONGLONG timeCur = GetTickCount64();
    if( timeStart == 0 )
        timeStart = timeCur;
    t = ( timeCur - timeStart ) / 1000.0f;
	//m_World = XMMatrixRotationY( t );

	// need temp variables on stack to get past bug : 
	// http://www.gamedev.net/topic/627033-xmmatrixtranspose-crashes/

	XMMATRIX world = m_World;
	XMMATRIX view = camera->GetViewMatrix();
	XMMATRIX projection = m_Projection;

	ConstantBufferValues constantBuff;
	constantBuff.mWorld = DirectX::XMMatrixTranspose(world);
	constantBuff.mView = DirectX::XMMatrixTranspose(view);
	constantBuff.mProjection = DirectX::XMMatrixTranspose(projection);
	d3dDeviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuff, 0, 0);
}

void MT_ConstantBuffer::Init(ID3D11Device *d3dDevice, UINT screenWidth, UINT screenHeight)
{
	InitMatrices(screenWidth, screenHeight);
	InitConstantBuffer(d3dDevice);
}

void MT_ConstantBuffer::Update(ID3D11DeviceContext *d3dDeviceContext, MT_Camera *camera) 
{
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	UpdateConstantBuffer(d3dDeviceContext, camera);
}

void MT_ConstantBuffer::Clean() 
{
	if(m_constantBuffer) m_constantBuffer->Release();
}