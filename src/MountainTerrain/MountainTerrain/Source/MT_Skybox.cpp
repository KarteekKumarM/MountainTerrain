#include "MT_Skybox.h"
#include "MT_Utility.h"
#include "MT_Logger.h"
#include "MT_Settings.h"

void MT_Skybox::InitVertices()
{
	// Y is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_TOP][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_TOP][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_TOP][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_TOP][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);

	// Y is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_BOTTOM][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BOTTOM][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BOTTOM][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BOTTOM][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);

	// X is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_LEFT][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_LEFT][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_LEFT][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_LEFT][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);

	// X is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_RIGHT][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_RIGHT][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_RIGHT][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_RIGHT][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);

	// Z is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_FRONT][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_FRONT][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_FRONT][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_FRONT][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / 2);

	// Z is fixed
	m_verticesRelativeToOrigin[SKYBOX_FACE_BACK][SKYBOX_VERT_UL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BACK][SKYBOX_VERT_UR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / 2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BACK][SKYBOX_VERT_LL] = XMFLOAT3(SKYBOX_WIDTH / -2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);
	m_verticesRelativeToOrigin[SKYBOX_FACE_BACK][SKYBOX_VERT_LR] = XMFLOAT3(SKYBOX_WIDTH / 2, SKYBOX_HEIGHT / -2, SKYBOX_DEPTH / -2);
}

enum VertDirection
{
	VERT_DIRECTION_CLOCKWISE,
	VERT_DIRECTION_ANTI_CLOCKWISE
};

void MT_Skybox::InitIndices()
{
	for (int i = 0; i < SKYBOX_FACE_COUNT; i++)
	{
		int indexIntoIndexArray = 6 * i;
		int indexIntoVerticesArray = SKYBOX_VERT_COUNT * i;

		VertDirection direction = VERT_DIRECTION_CLOCKWISE;
		switch (i)
		{
		case SKYBOX_FACE_BOTTOM:
		case SKYBOX_FACE_LEFT:
		case SKYBOX_FACE_FRONT:
			direction = VERT_DIRECTION_ANTI_CLOCKWISE;
			break;
		case SKYBOX_FACE_TOP:
		case SKYBOX_FACE_RIGHT:
		case SKYBOX_FACE_BACK:
		default:
			direction = VERT_DIRECTION_CLOCKWISE;
		}

		// clockwise vs. anticlockwise
		if (direction == VERT_DIRECTION_CLOCKWISE)		// can toggle this to draw outside box
		{
			// triangle 1
			m_indices[indexIntoIndexArray + 0] = indexIntoVerticesArray + SKYBOX_VERT_UL;
			m_indices[indexIntoIndexArray + 1] = indexIntoVerticesArray + SKYBOX_VERT_UR;
			m_indices[indexIntoIndexArray + 2] = indexIntoVerticesArray + SKYBOX_VERT_LL;
			// triangle 2
			m_indices[indexIntoIndexArray + 3] = indexIntoVerticesArray + SKYBOX_VERT_UR;
			m_indices[indexIntoIndexArray + 4] = indexIntoVerticesArray + SKYBOX_VERT_LR;
			m_indices[indexIntoIndexArray + 5] = indexIntoVerticesArray + SKYBOX_VERT_LL;
		}
		else
		{
			// triangle 1
			m_indices[indexIntoIndexArray + 0] = indexIntoVerticesArray + SKYBOX_VERT_LL;
			m_indices[indexIntoIndexArray + 1] = indexIntoVerticesArray + SKYBOX_VERT_UR;
			m_indices[indexIntoIndexArray + 2] = indexIntoVerticesArray + SKYBOX_VERT_UL;
			// triangle 2
			m_indices[indexIntoIndexArray + 3] = indexIntoVerticesArray + SKYBOX_VERT_LL;
			m_indices[indexIntoIndexArray + 4] = indexIntoVerticesArray + SKYBOX_VERT_LR;
			m_indices[indexIntoIndexArray + 5] = indexIntoVerticesArray + SKYBOX_VERT_UR;
		}
	}
}

void MT_Skybox::Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext)
{
	m_shader = new MT_Shader();
	m_shader->Init(d3dDevice, d3dDeviceContext, SKYBOX_VERTEX_SHADER_PATH, SKYBOX_PIXEL_SHADER_PATH);

	InitVertices();
	InitIndices();
	Update(XMFLOAT3(0,0,0));
	LoadVertexBuffer(d3dDevice);
	LoadIndexBuffer(d3dDevice);

	m_texture = new MT_Texture();
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_TOP);
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_BOTTOM);
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_FRONT);
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_BACK);
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_LEFT);
	m_texture->LoadTexture(d3dDevice, d3dDeviceContext, SKYBOX_TEXTURE_RIGHT);
	m_texture->LoadSampler(d3dDevice, d3dDeviceContext);
}

void MT_Skybox::LoadVertexBuffer(ID3D11Device *d3dDevice)
{
	// desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc)); 
	vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;											// write access access by CPU and GPU
	vertexBuffDesc.ByteWidth = sizeof(SkyboxVertex) * SKYBOX_NUM_VERTICES;				// size is the VERTEX struct
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;								// use as a vertex buffer
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;								// allow CPU to write in buffer

	// buffer
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = m_verticesRelativeToPlayer;
	HRESULT hr = d3dDevice->CreateBuffer(&vertexBuffDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Skybox - Unable to create vertex buffer", L"Error", MB_OK);
	}
}

void MT_Skybox::LoadIndexBuffer(ID3D11Device *d3dDevice)
{
	// description
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * SKYBOX_NUM_INDICES;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	// buffer
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = m_indices;
	HRESULT hr = d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Skybox - Unable to create index buffer", L"Error", MB_OK);
	}
}

void MT_Skybox::Update(XMFLOAT3 playerPosition)
{
	for (UINT i = 0; i < SKYBOX_FACE_COUNT; i++)
	{
		for (UINT j = 0; j < SKYBOX_VERT_COUNT; j++)
		{
			SkyboxVertex svert;
			svert.Position = XMFloat3Add(m_verticesRelativeToOrigin[i][j], playerPosition);
			m_verticesRelativeToPlayer[(i * SKYBOX_VERT_COUNT) + j] = svert;
		}
	}
	for (UINT i = 0; i < SKYBOX_FACE_COUNT; i++)
	{
		m_verticesRelativeToPlayer[(i * SKYBOX_VERT_COUNT) + SKYBOX_VERT_UL].TextureCoord = XMFLOAT3(0.0, 1.0, FLOAT(i));
		m_verticesRelativeToPlayer[(i * SKYBOX_VERT_COUNT) + SKYBOX_VERT_UR].TextureCoord = XMFLOAT3(1.0, 1.0, FLOAT(i));
		m_verticesRelativeToPlayer[(i * SKYBOX_VERT_COUNT) + SKYBOX_VERT_LL].TextureCoord = XMFLOAT3(0.0, 0.0, FLOAT(i));
		m_verticesRelativeToPlayer[(i * SKYBOX_VERT_COUNT) + SKYBOX_VERT_LR].TextureCoord = XMFLOAT3(1.0, 0.0, FLOAT(i));
	}
}

void MT_Skybox::SetShadersActive(ID3D11DeviceContext *d3dDeviceContext)
{
	m_shader->SetAsActive(d3dDeviceContext);
	m_texture->SetShaderResources(d3dDeviceContext);
}

void MT_Skybox::RenderFrame(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext)
{
	// d3dDeviceContext->UpdateSubresource(m_vertexBuffer, 0, NULL, &m_verticesRelativeToPlayer, 0, 0);
	// FIX ME : Will run out of memory - creating new buffers every frame... ROFL
	LoadVertexBuffer(d3dDevice);

	//// select which vertex buffer to display
	UINT stride = sizeof(SkyboxVertex);
	UINT offset = 0;

	// set to use vertex bufer and index buffer
	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// select which primtive type we are using
	d3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer using the index buffer
	d3dDeviceContext->DrawIndexed(SKYBOX_NUM_INDICES, 0, 0);
}

void MT_Skybox::Clean()
{
	m_texture->Clean();
	delete m_texture;
	m_texture = 0;

	m_shader->Clean();
	delete m_shader;
	m_shader = 0;
}