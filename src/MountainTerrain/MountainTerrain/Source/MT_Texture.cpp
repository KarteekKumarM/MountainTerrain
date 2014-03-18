#include "MT_Texture.h"
#include "MT_Logger.h"

void MT_Texture::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* textureFileName)
{
	m_imageReader = new MT_BitmapReader();
	m_imageReader->Init(textureFileName);

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_imageReader->width();
	texDesc.Height = m_imageReader->height();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA texData;
	texData.pSysMem = m_imageReader->data();
	texData.SysMemPitch = static_cast<UINT>(m_imageReader->width() * 3);
	texData.SysMemSlicePitch = static_cast<UINT>(m_imageReader->width() * m_imageReader->height() * 9); // not needed since this is 2d


	HRESULT hr = d3dDevice->CreateTexture2D(&texDesc, &texData, &m_texture);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to load texture");
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	memset(&SRVDesc, 0, sizeof(SRVDesc));
	SRVDesc.Format = texDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	hr = d3dDevice->CreateShaderResourceView(m_texture, &SRVDesc, &m_shaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to create texture shader resource view");
	}

	d3dDeviceContext->PSSetShaderResources(0, 1, &m_shaderResourceView);

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = d3dDevice->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to create texture sampler state");
	}

	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void MT_Texture::Clean()
{
	m_imageReader->Clean();
	delete m_imageReader;
	m_imageReader = 0;

	m_texture->Release();
	m_texture = 0;
}