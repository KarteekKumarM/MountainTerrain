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
	else 
	{
		// load the shader and setup the shader obj
		d3dDevice->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), NULL, &m_d3dVertexShader);

		// set the shader as active
		d3dDeviceContext->VSSetShader(m_d3dVertexShader, 0, 0);
	}

	// -------- PIXEL SHADER
	// compile pixel shader
	hr = D3DReadFileToBlob( pixelShaderFileName, &m_pixelShaderBlob );
	if(FAILED(hr))
	{
		MT_Logger::LogError("Unable to compile pixel shader");
		return hr;
	} 
	else
	{
		// load the shader and setup the shader obj
		d3dDevice->CreatePixelShader(m_pixelShaderBlob->GetBufferPointer(), m_pixelShaderBlob->GetBufferSize(), NULL, &m_d3dPixelShader);

		// set the shader as active
		d3dDeviceContext->PSSetShader(m_d3dPixelShader, 0, 0);
	}

	return hr;
}

void MT_Shader::Clean() {
	if(m_vertexShaderBlob) m_vertexShaderBlob->Release();
	if(m_pixelShaderBlob) m_pixelShaderBlob->Release();

	if(m_d3dVertexShader) m_d3dVertexShader->Release();
	if(m_d3dPixelShader) m_d3dPixelShader->Release();
}