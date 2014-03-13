#include "MT_Terrain.h"
#include "stdio.h"

void MT_Terrain::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{
	m_shader = new MT_Shader();
	m_shader->Init(d3dDevice, d3dDeviceContext);

	m_heightMap = new MT_HeightMap();
	m_heightMap->Init("Resources/HeightMapImage.bmp");

	CreateInputLayoutObjectForVertexBuffer(d3dDevice, d3dDeviceContext, m_shader->GetVertexShaderBlob());

	LoadVertexBuffer(d3dDevice, d3dDeviceContext);
	LoadIndexBuffer(d3dDevice, d3dDeviceContext);
}

void MT_Terrain::CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob) 
{
	// create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12 + 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    d3dDevice->CreateInputLayout(ied, 3, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_layout);
    d3dDeviceContext->IASetInputLayout(m_layout);
}

void MT_Terrain::LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{
	UINT numOfVertices = m_heightMap->width() * m_heightMap->height();

	TerrainVertex *vertices = new TerrainVertex[numOfVertices];

	XMFLOAT3 default_normal = XMFLOAT3(0, 1, 0);

	// center at 0,0
	FLOAT minX = -1.0f * (m_heightMap->width() / 2);
	FLOAT minY = -1.0f * (m_heightMap->height() / 2);

	for(UINT i = 0; i < m_heightMap->height(); i++) {
		for(UINT j = 0; j < m_heightMap->width(); j++) {
			int index = (i * m_heightMap->width()) + j;
			FLOAT x = ( minX + j ) * k_SingleCellWidth;
			FLOAT z = ( minY + i ) * k_SingleCellDepth;
			FLOAT y = m_heightMap->heightAt(i, j);
			vertices[index].Position = XMFLOAT3(x, y, z);
			vertices[index].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if( i != 0 && j != 0 ) 
			{
				// Calculate normal
				IndicesOfTwoTrianglesThatFormACell indiciesForThisPoint = m_heightMap->getIndiciesOfTheTwoTrianglesThatFormACellAtPoint(i, j);

				HeightMapType heightStruct_UR = m_heightMap->heightMapStructAt( indiciesForThisPoint.indexOf_UR );
				XMFLOAT3 vertex_UR = XMFLOAT3((FLOAT)heightStruct_UR.x, heightStruct_UR.height, (FLOAT)heightStruct_UR.z);
				XMVECTOR vector_vertex_UR = XMLoadFloat3(&vertex_UR);

				HeightMapType heightStruct_LL = m_heightMap->heightMapStructAt( indiciesForThisPoint.indexOf_LL );
				XMFLOAT3 vertex_LL = XMFLOAT3((FLOAT)heightStruct_LL.x, heightStruct_LL.height, (FLOAT)heightStruct_LL.z);
				XMVECTOR vector_vertex_LL = XMLoadFloat3(&vertex_LL);

				HeightMapType heightStruct_LR = m_heightMap->heightMapStructAt( indiciesForThisPoint.indexOf_LR );
				XMFLOAT3 vertex_LR = XMFLOAT3((FLOAT)heightStruct_LR.x, heightStruct_LR.height, (FLOAT)heightStruct_LR.z);
				XMVECTOR vector_vertex_LR = XMLoadFloat3(&vertex_LR);

				XMVECTOR normal = XMVector3Normalize(XMVector3Cross( vector_vertex_LL - vector_vertex_UR, vector_vertex_LR - vector_vertex_UR ));
				XMStoreFloat3(&vertices[index].Normal, normal);
			}
			else
			{
				vertices[index].Normal = default_normal;
			}
		}
	}

	// desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
    vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
	vertexBuffDesc.ByteWidth = sizeof(TerrainVertex) * numOfVertices;			// size is the VERTEX struct * 3
    vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
    vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

	// buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	d3dDevice->CreateBuffer(&vertexBuffDesc, &vertexData, &m_vertexBuffer);

	delete[] vertices;
	vertices = 0;
}

void MT_Terrain::LoadIndexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{
	// Create index buffer
	int indexCount = 0;
	UINT numberOfIndices = m_heightMap->width() * m_heightMap->height() * 6;

	WORD *indices = new WORD[numberOfIndices];

	for(UINT i = 1; i < m_heightMap->height(); i++)
	{
		for(UINT j = 1; j < m_heightMap->width(); j++)
		{
			IndicesOfTwoTrianglesThatFormACell indiciesForThisPoint = m_heightMap->getIndiciesOfTheTwoTrianglesThatFormACellAtPoint(i, j);

			// first triangle
			indices[indexCount++] = indiciesForThisPoint.indexOf_UR;
			indices[indexCount++] = indiciesForThisPoint.indexOf_LR;
			indices[indexCount++] = indiciesForThisPoint.indexOf_LL;

			// second triangle
			indices[indexCount++] = indiciesForThisPoint.indexOf_LL;
			indices[indexCount++] = indiciesForThisPoint.indexOf_UL;
			indices[indexCount++] = indiciesForThisPoint.indexOf_UR;
		}
	}

	// description
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * numberOfIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// buffer
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices;
	HRESULT hr = d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(hr))
	{
		MessageBox(nullptr, L"Unable to create index buffer", L"Error", MB_OK);
	}

	delete[] indices;
	indices = 0;
}

void MT_Terrain::RenderFrame(ID3D11DeviceContext *d3dDeviceContext) 
{
	// select which vertex buffer to display
	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;

	// set to use vertex bufer and index buffer
	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// select which primtive type we are using
	d3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer using the index buffer
	d3dDeviceContext->DrawIndexed(6 * m_heightMap->height() * m_heightMap->width(), 0, 0);
}

void MT_Terrain::Clean() 
{
	m_heightMap->Clean();
	delete m_heightMap;
	m_heightMap = 0;

	if(m_vertexBuffer) m_vertexBuffer->Release();
	if(m_indexBuffer) m_indexBuffer->Release();

	if(m_layout) m_layout->Release();

	m_shader->Clean();
	delete m_shader;
	m_shader = 0;
}