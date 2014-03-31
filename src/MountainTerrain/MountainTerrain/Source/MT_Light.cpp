#include "MT_Light.h"
#include "MT_Settings.h"

void MT_Light::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, LightBufferValues lightBufferValues)
{
	m_d3dDeviceContext = d3dDeviceContext;
	m_lightBufferValues = lightBufferValues;

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

	UpdateLightBuffer();
}

void MT_Light::UpdateLightBuffer()
{
	// copy the values of the light into the light constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_d3dDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Unable to update light buffer", L"Error", MB_OK);
		return;
	}
	LightBufferValues* mappedLightBufferValues = (LightBufferValues*)mappedResource.pData;
	mappedLightBufferValues->ambient = m_lightBufferValues.ambient;
	mappedLightBufferValues->diffuse = m_lightBufferValues.diffuse;
	mappedLightBufferValues->direction = m_lightBufferValues.direction;
	mappedLightBufferValues->light_enabled = m_lightBufferValues.light_enabled;
	mappedLightBufferValues->texture_enabled = m_lightBufferValues.texture_enabled;
	m_d3dDeviceContext->Unmap(m_lightBuffer, 0);

	m_d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);
}

void MT_Light::Toggle( bool shouldToggleLight, bool shouldToggleTexture )
{
	if (shouldToggleLight)
	{
		m_lightBufferValues.light_enabled = !m_lightBufferValues.light_enabled;
	}

	if (shouldToggleTexture)
	{
		m_lightBufferValues.texture_enabled = !m_lightBufferValues.texture_enabled;
	}
	
	UpdateLightBuffer();
}

void MT_Light::Clean()
{
	if (m_lightBuffer) m_lightBuffer->Release();
	m_lightBuffer = 0;
}