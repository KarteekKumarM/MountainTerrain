#include "MT_Shader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

HRESULT MT_Shader::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{

	const LPCWSTR VERTEX_SHADER_FILENAME = L"Shaders/SimpleShader.hlsl";
	const LPCSTR VERTEX_SHADER_ENTRYPOINT = "VS";
	const LPCSTR VERTEX_SHADER_MODEL = "vs_5_0";

	const LPCWSTR PIXEL_SHADER_FILENAME = L"Shaders/SimpleShader.hlsl";
	const LPCSTR PIXEL_SHADER_ENTRYPOINT = "PS";
	const LPCSTR PIXEL_SHADER_MODEL = "ps_5_0";

	HRESULT hr = S_OK;

	// -------- VERTEX SHADER
	// compile vertex shader
	hr = CompileShaderFromFile(VERTEX_SHADER_FILENAME, VERTEX_SHADER_ENTRYPOINT, VERTEX_SHADER_MODEL, &m_vertexShaderBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr, L"Unable to compile vertex shader", L"Error", MB_OK);
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
	hr = CompileShaderFromFile(PIXEL_SHADER_FILENAME, PIXEL_SHADER_ENTRYPOINT, PIXEL_SHADER_MODEL, &m_pixelShaderBlob);
	if(FAILED(hr))
	{
		MessageBox(nullptr, L"Unable to compile pixel shader", L"Error", MB_OK);
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

// reusable function to compile vertex and pixel shaders
HRESULT MT_Shader::CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut) {
	HRESULT hr = S_OK;

	// setup shader flags
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// better shader debugging
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *errorBlob = nullptr;
	hr = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, shaderModel, dwShaderFlags, 0, blobOut, &errorBlob);
	if(FAILED(hr))
	{
		if(errorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
		}
	}

	if(errorBlob)
	{
		errorBlob->Release();
	}

	return hr;
}