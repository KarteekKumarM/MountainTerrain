#include "MT_Texture.h"
#include "MT_Logger.h"

#include "WICTextureLoader.h"
#pragma comment(lib, "DirectXTK.lib")
using namespace DirectX;

#include <string>

void MT_Texture::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, char* grasstextureFileName, char *rockTextureFileName, char *waterTextureFileName)
{

	std::string strGrassTexFilename(grasstextureFileName);
	std::wstring strGrassTexFilenameW(strGrassTexFilename.begin(), strGrassTexFilename.end());

	HRESULT hr = CreateWICTextureFromFile(d3dDevice, d3dDeviceContext, strGrassTexFilenameW.c_str(), reinterpret_cast<ID3D11Resource**>(&m_grassTexture), &m_grassTexShaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("MT_Texture : Unable to grass create texture - file : %s ", strGrassTexFilename);
		return;
	}

	std::string strRockTexFilename(rockTextureFileName);
	std::wstring strRockTexFilenameW(strRockTexFilename.begin(), strRockTexFilename.end());

	hr = CreateWICTextureFromFile(d3dDevice, d3dDeviceContext, strRockTexFilenameW.c_str(), reinterpret_cast<ID3D11Resource**>(&m_rockTexture), &m_rockTexShaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("MT_Texture : Unable to rock create texture - file : %s ", strRockTexFilename);
		return;
	}

	std::string strWaterTexFilename(waterTextureFileName);
	std::wstring strWaterTexFilenameW(strWaterTexFilename.begin(), strWaterTexFilename.end());

	hr = CreateWICTextureFromFile(d3dDevice, d3dDeviceContext, strWaterTexFilenameW.c_str(), reinterpret_cast<ID3D11Resource**>(&m_waterTexture), &m_waterTexShaderResourceView);
	if (FAILED(hr))
	{
		MT_Logger::LogError("MT_Texture : Unable to water create texture - file : %s ", waterTextureFileName);
		return;
	}

	ID3D11ShaderResourceView *shaderResourceViewArray[3] = { m_grassTexShaderResourceView, m_rockTexShaderResourceView, m_waterTexShaderResourceView };

	d3dDeviceContext->PSSetShaderResources(0, 3, shaderResourceViewArray);
	//d3dDeviceContext->PSSetShaderResources(1, 2, &m_rockTexShaderResourceView);
	//d3dDeviceContext->PSSetShaderResources(2, 3, &m_waterTexShaderResourceView);

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
	if (m_grassTexture) m_grassTexture->Release();
	m_grassTexture = 0;

	if (m_rockTexture) m_rockTexture->Release();
	m_rockTexture = 0;

	if (m_waterTexture) m_waterTexture->Release();
	m_waterTexture = 0;

	if (m_grassTexShaderResourceView) m_grassTexShaderResourceView->Release();
	m_grassTexShaderResourceView = 0;

	if (m_rockTexShaderResourceView) m_rockTexShaderResourceView->Release();
	m_rockTexShaderResourceView = 0;

	if (m_waterTexShaderResourceView) m_waterTexShaderResourceView->Release();
	m_waterTexShaderResourceView = 0;

	if (m_samplerState) m_samplerState->Release();
	m_samplerState = 0;
}