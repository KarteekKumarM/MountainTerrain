#include "MT_Texture.h"
#include "MT_Logger.h"

#include <string>

#include "WICTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;

void MT_Texture::LoadTexture(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* textureFileName)
{
	std::string strTexFilename(textureFileName);
	std::wstring strTexFilenameW(strTexFilename.begin(), strTexFilename.end());
	ID3D11Texture2D *texture;
	ID3D11ShaderResourceView *shaderResourceView;
	HRESULT hr = CreateWICTextureFromFile(d3dDevice, d3dDeviceContext, strTexFilenameW.c_str(), reinterpret_cast<ID3D11Resource**>(&texture), &shaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("MT_Texture : Unable to grass create texture - file : %s ", textureFileName);
		return;
	}
	m_textures.push_back(texture);
	m_shaderResourceViews.push_back(shaderResourceView);
}

void MT_Texture::SetShaderResources(ID3D11DeviceContext *d3dDeviceContext)
{
	ID3D11ShaderResourceView **shaderResourceViewArray = new ID3D11ShaderResourceView*[m_shaderResourceViews.size()];
	for (unsigned int i = 0; i < m_shaderResourceViews.size(); i++)
		shaderResourceViewArray[i] = m_shaderResourceViews[i];
	d3dDeviceContext->PSSetShaderResources(0, m_shaderResourceViews.size(), shaderResourceViewArray);
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void MT_Texture::LoadSampler(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext)
{
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
	HRESULT hr = d3dDevice->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to create texture sampler state");
		return;
	}
}

void MT_Texture::Clean()
{
	for (int i = m_textures.size() - 1; i >= 0; i--)
	{
		ID3D11Texture2D* texture = m_textures[i];
		m_textures.pop_back();
		texture->Release();
		texture = 0;
	}

	for (int i = m_shaderResourceViews.size() - 1; i >= 0; i--)
	{
		ID3D11ShaderResourceView* shaderResourceView = m_shaderResourceViews[i];
		m_shaderResourceViews.pop_back();
		shaderResourceView->Release();
		shaderResourceView = 0;
	}

	if (m_samplerState) m_samplerState->Release();
	m_samplerState = 0;
}