#include "MT_Light.h"

void MT_Light::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, LightBufferValues lightBufferValues)
{
	// create the constant buffer for light
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferValues);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	HRESULT hr = d3dDevice->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"Unable to create light buffer", L"Error", MB_OK);
		return;
	}

	// copy the values of the light into the light constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = d3dDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Unable to create light buffer", L"Error", MB_OK);
		return;
	}
	LightBufferValues* mappedLightBufferValues = (LightBufferValues*)mappedResource.pData;
	mappedLightBufferValues->ambient = lightBufferValues.ambient;
	mappedLightBufferValues->diffuse = lightBufferValues.diffuse;
	mappedLightBufferValues->direction = lightBufferValues.direction;
	mappedLightBufferValues->padding = 0.0f;
	d3dDeviceContext->Unmap(m_lightBuffer, 0);

	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);
}
void MT_Light::Clean()
{
	if (m_lightBuffer) m_lightBuffer->Release();
	m_lightBuffer = 0;
}