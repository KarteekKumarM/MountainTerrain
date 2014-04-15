#include "MT_Terrain.h"
#include "MT_Utility.h"
#include "MT_Settings.h"
#include "MT_Profiler.h"

const LPCWSTR MT_Terrain::k_VertexShaderFileName = VERTEX_SHADER_PATH;
const LPCWSTR MT_Terrain::k_PixelShaderFileName = PIXEL_SHADER_PATH;
const FLOAT MT_Terrain::k_SeaLevel = SEA_LEVEL;
const FLOAT MT_Terrain::k_SingleCellWidth = GRID_CELL_WIDTH;
const FLOAT MT_Terrain::k_SingleCellDepth = GRID_CELL_DEPTH;

const XMFLOAT4 MT_Terrain::k_Light_Ambient = LIGHT_AMBIENT_INTENSITY;
const XMFLOAT4 MT_Terrain::k_Light_Diffuse = LIGHT_DIFFUSE_INTENSITY;
const XMFLOAT3 MT_Terrain::k_Light_Direction = LIGHT_DIFFUSE_DIRECTION;

void MT_Terrain::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{
	m_shader = new MT_Shader();
	m_shader->Init(d3dDevice, d3dDeviceContext, k_VertexShaderFileName, k_PixelShaderFileName);

	m_heightMap = new MT_HeightMap();
	m_heightMap->Init(HEIGHTMAP_PATH);

	m_texture = new MT_Texture();
	m_texture->Init(d3dDevice, d3dDeviceContext, TEXTURE_GRASS_PATH, TEXTURE_ROCK_PATH, TEXTURE_WATER_PATH);

	m_light = new MT_Light();
	LightBufferValues lightValues;
	lightValues.ambient = k_Light_Ambient;
	lightValues.diffuse = k_Light_Diffuse;
	lightValues.direction = k_Light_Direction;
	lightValues.light_enabled = LIGHT_TOGGLE;
	lightValues.texture_enabled = TEXTURE_TOGGLE;
	m_light->Init(d3dDevice, d3dDeviceContext, lightValues);

	CreateInputLayoutObjectForVertexBuffer(d3dDevice, d3dDeviceContext, m_shader->GetVertexShaderBlob());

	m_toggle_flatGrid = TOGGLE_FLAT_GRID;
	LoadVertexBuffer(d3dDevice, d3dDeviceContext);
	LoadIndexBuffer(d3dDevice, d3dDeviceContext);
}

void MT_Terrain::CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob) 
{
	// create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    d3dDevice->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &m_layout);
    d3dDeviceContext->IASetInputLayout(m_layout);
}

void MT_Terrain::LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) 
{
	UINT numOfVertices = m_heightMap->width() * m_heightMap->height();

	TerrainVertex *vertices = new TerrainVertex[numOfVertices];

	XMFLOAT3 default_normal = XMFLOAT3_UP;

	// center at 0,0
	FLOAT minX = -1.0f * (m_heightMap->width() / 2);
	FLOAT minY = -1.0f * (m_heightMap->height() / 2);

	// Set position
	for(UINT j = 0; j < m_heightMap->height(); j++) 
	{
		for(UINT i = 0; i < m_heightMap->width(); i++) 
		{
			XMFLOAT3 heightVertex = m_heightMap->heightAt(i, j);
			FLOAT x = ( minX + heightVertex.x ) * k_SingleCellWidth;
			FLOAT z = ( minY + heightVertex.z ) * k_SingleCellDepth;
			FLOAT y = heightVertex.y >= k_SeaLevel && !m_toggle_flatGrid ? heightVertex.y : k_SeaLevel;

			int index = m_heightMap->indexOf(i, j);
			vertices[index].Position = XMFLOAT3(x, y, z);
		}
	}

	MT_Profiler::shared()->RecordNumberOfVertices(numOfVertices);

	// Calc normals
	XMFLOAT3 *normals = new XMFLOAT3[numOfVertices];
	for(UINT i = 0; i < m_heightMap->height(); i++) 
	{
		for(UINT j = 0; j < m_heightMap->width(); j++) 
		{
			int index = m_heightMap->indexOf(i, j);

			if ( i != 0 && j != 0  )
			{
				IndicesOfTwoTrianglesThatFormACell indiciesForThisPoint = m_heightMap->getIndiciesOfTheTwoTrianglesThatFormACellAtPoint(i, j);

				// triangle 1
				XMFLOAT3 a = XMFloat3Subtract(vertices[indiciesForThisPoint.indexOf_LL].Position, vertices[indiciesForThisPoint.indexOf_UL].Position);
				XMFLOAT3 b = XMFloat3Subtract(vertices[indiciesForThisPoint.indexOf_UR].Position, vertices[indiciesForThisPoint.indexOf_UL].Position);
				XMFLOAT3 ab = XMFloat3Cross(a, b);

				// triangle 2
				XMFLOAT3 c = XMFloat3Subtract(vertices[indiciesForThisPoint.indexOf_LL].Position, vertices[indiciesForThisPoint.indexOf_UR].Position);
				XMFLOAT3 d = XMFloat3Subtract(vertices[indiciesForThisPoint.indexOf_LR].Position, vertices[indiciesForThisPoint.indexOf_UR].Position);
				XMFLOAT3 cd = XMFloat3Cross(c, d);

				normals[index] = XMFloat3Average( ab, cd );
			}
			else
			{
				normals[index] = default_normal;
			}
		}
	}

	// Set normal by taking averages - smooth light transitions
	for(UINT i = 0; i < m_heightMap->height(); i++) 
	{
		for(UINT j = 0; j < m_heightMap->width(); j++) 
		{

			// this box, LL
			int count = 1;
			int index = m_heightMap->indexOf(i, j);
			XMFLOAT3 normalSum = normals[index];

			// LR
			if( j < m_heightMap->width() - 1 ) 
			{
				count++;
				int index_LR = m_heightMap->indexOf(i, j + 1);
				normalSum = XMFloat3Add( normals[index_LR], normalSum );
			}

			// UL
			if( i < m_heightMap->height() - 1 ) 
			{
				count++;
				int index_UL = m_heightMap->indexOf(i + 1, j);
				normalSum = XMFloat3Add( normals[index_UL], normalSum );
			}

			// UR
			if( i < m_heightMap->height() - 1 && j < m_heightMap->width() - 1 ) 
			{
				count++;
				int index_UR = m_heightMap->indexOf(i + 1, j + 1);
				normalSum = XMFloat3Add( normals[index_UR], normalSum );
			}

			// calc average
			vertices[index].Normal = XMFloat3Divide( (FLOAT)count, normalSum );
		}
	}

	// desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
    vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
	vertexBuffDesc.ByteWidth = sizeof(TerrainVertex) * numOfVertices;			// size is the VERTEX struct
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

	MT_Profiler::shared()->RecordNumberOfTriangles(numberOfIndices/3);

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

void MT_Terrain::ProcessInput(MT_InputHandler *inputHandler)
{
	m_light->Toggle(inputHandler->IsLightToggleKeyPressed(), inputHandler->IsTextureToggleKeyPressed());
}

void MT_Terrain::Clean() 
{
	m_light->Clean();
	delete m_light;
	m_light = 0;

	m_heightMap->Clean();
	delete m_heightMap;
	m_heightMap = 0;

	m_texture->Clean();
	delete m_texture;
	m_texture = 0;

	if(m_vertexBuffer) m_vertexBuffer->Release();
	if(m_indexBuffer) m_indexBuffer->Release();

	if(m_layout) m_layout->Release();

	m_shader->Clean();
	delete m_shader;
	m_shader = 0;
}