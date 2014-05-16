#include "MT_Shader.h"

#include <stdio.h>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "MT_Logger.h"

HRESULT MT_Shader::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, const LPCWSTR vertexShaderFileName, const LPCWSTR pixelShaderFileName) 
{
	HRESULT hr = S_OK;

	// -------- VERTEX SHADER
	hr = D3DReadFileToBlob( vertexShaderFileName, &m_vertexShaderBlob );
	if(FAILED(hr))
	{
		MT_Logger::LogError("Unable to compile vertex shader");
		return hr;
	} 
	// load the shader and setup the shader obj
	hr = d3dDevice->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), NULL, &m_d3dVertexShader);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to create vertex shader");
		return hr;
	}

	// -------- PIXEL SHADER
	// compile pixel shader
	hr = D3DReadFileToBlob( pixelShaderFileName, &m_pixelShaderBlob );
	if(FAILED(hr))
	{
		MT_Logger::LogError("Unable to compile pixel shader");
		return hr;
	} 
	// load the shader and setup the shader obj
	hr = d3dDevice->CreatePixelShader(m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize(), NULL, &m_d3dPixelShader);
	if (FAILED(hr))
	{
		MT_Logger::LogError("Unable to create pixel shader");
		return hr;
	}
	return hr;
}

void MT_Shader::SetAsActive(ID3D11DeviceContext *d3dDeviceContext)
{
	d3dDeviceContext->VSSetShader(m_d3dVertexShader, 0, 0);
	d3dDeviceContext->PSSetShader(m_d3dPixelShader, 0, 0);
}

void MT_Shader::Clean() {
	if(m_vertexShaderBlob) m_vertexShaderBlob->Release();
	if(m_pixelShaderBlob) m_pixelShaderBlob->Release();

	if(m_d3dVertexShader) m_d3dVertexShader->Release();
	if(m_d3dPixelShader) m_d3dPixelShader->Release();
}