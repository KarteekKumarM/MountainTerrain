#include "MT_Terrain.h"
#include "stdio.h"

void MT_Terrain::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) {
	// ZERO UP
	for(int i = 0; i < k_HeightMapDepth; i++) {
		for(int j = 0; j < k_HeightMapWidth; j++) {
			m_heightMap[i][j] = 0;
		}
	}

	m_shader = new MT_Shader();
	m_shader->Init(d3dDevice, d3dDeviceContext);

	CreateInputLayoutObjectForVertexBuffer(d3dDevice, d3dDeviceContext, m_shader->GetVertexShaderBlob());

	LoadVertexBuffer(d3dDevice, d3dDeviceContext);
	LoadIndexBuffer(d3dDevice, d3dDeviceContext);
}

void MT_Terrain::CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob) {
	// create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    d3dDevice->CreateInputLayout(ied, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_layout);
    d3dDeviceContext->IASetInputLayout(m_layout);
}

void MT_Terrain::LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) {
	TerrainVertex vertices[k_HeightMapWidth * k_HeightMapDepth];
	for(int i = 0; i < k_HeightMapDepth; i++) {
		for(int j = 0; j < k_HeightMapWidth; j++) {
			int index = (i * k_HeightMapWidth) + j;
			vertices[index].Position = XMFLOAT3(k_SingleCellWidth * j, m_heightMap[i][j], k_SingleCellDepth * i);
			vertices[index].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
    vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
	vertexBuffDesc.ByteWidth = sizeof(TerrainVertex)*k_HeightMapDepth*k_HeightMapWidth;			// size is the VERTEX struct * 3
    vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
    vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

	// buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	d3dDevice->CreateBuffer(&vertexBuffDesc, &vertexData, &m_vertexBuffer);
}

void MT_Terrain::LoadIndexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) {
	// Create index buffer
	int indexCount = 0;
	WORD indices[k_HeightMapWidth * k_HeightMapDepth * 6];
	for(int i = 1; i < k_HeightMapDepth; i++) {
		for(int j = 1; j < k_HeightMapWidth; j++) {

			WORD indexOf_UR = (i * k_HeightMapWidth) + j;
			WORD indexOf_UL = (i * k_HeightMapWidth) + (j-1);
			WORD indexOf_LL = ((i-1) * k_HeightMapWidth) + (j-1);
			WORD indexOf_LR = ((i-1) * k_HeightMapWidth) + j;

			// first triangle
			indices[indexCount++] = indexOf_UR;
			indices[indexCount++] = indexOf_LR;
			indices[indexCount++] = indexOf_LL;

			// second triangle
			indices[indexCount++] = indexOf_LL;
			indices[indexCount++] = indexOf_UL;
			indices[indexCount++] = indexOf_UR;
		}
	}

	// TEMP CODE -- Logging .. fix me
	/*for(int k = 0; k < (k_HeightMapWidth-1) * (k_HeightMapDepth-1) * 6; k++) {
		char str[256];
		sprintf_s(str, sizeof(str), "%d--%d \n", k, indices[k]);
		OutputDebugStringA(str);
	}*/

	// description
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * 6 * k_HeightMapDepth * k_HeightMapWidth;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// buffer
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices;
	HRESULT hr = d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(hr)) {
		MessageBox(nullptr, L"Unable to create index buffer", L"Error", MB_OK);
	}
}

void MT_Terrain::RenderFrame(ID3D11DeviceContext *d3dDeviceContext) {
	// select which vertex buffer to display
	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;

	// set to use vertex bufer and index buffer
	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// select which primtive type we are using
	d3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer using the index buffer
	d3dDeviceContext->DrawIndexed(6*k_HeightMapDepth*k_HeightMapWidth, 0, 0);
}

void MT_Terrain::Clean() {
	if(m_vertexBuffer) m_vertexBuffer->Release();
	if(m_indexBuffer) m_indexBuffer->Release();

	if(m_layout) m_layout->Release();

	m_shader->Clean();
	delete m_shader;
	m_shader = 0;
}