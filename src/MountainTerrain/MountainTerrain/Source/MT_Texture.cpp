#include "MT_Texture.h"
#include "MT_Logger.h"

#include "WICTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;

#include <string>

void MT_Texture::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* textureFileName)
{

	std::string strFilename(textureFileName);
	std::wstring strFilenameW(strFilename.begin(), strFilename.end());
	HRESULT hr = CreateWICTextureFromFile(d3dDevice, d3dDeviceContext, strFilenameW.c_str(), reinterpret_cast<ID3D11Resource**>(&m_texture), &m_shaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("MT_Texture : Unable to create texture file : %s ", textureFileName);
		return;
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
		return;
	}

	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void MT_Texture::Clean()
{
	if (m_texture) m_texture->Release();
	m_texture = 0;

	if (m_shaderResourceView) m_shaderResourceView->Release();
	m_shaderResourceView = 0;

	if (m_samplerState) m_samplerState->Release();
	m_samplerState = 0;
}