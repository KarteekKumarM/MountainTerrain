#ifndef MT_TERRAIN_H
#define MT_TERRAIN_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_Shader.h"
#include "MT_HeightMap.h"

//static const UINT k_HeightMapWidth = 100;
//static const UINT k_HeightMapDepth = 100;

static const FLOAT k_SingleCellWidth = 1.0f;
static const FLOAT k_SingleCellDepth = 1.0f;

struct TerrainVertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

class MT_Terrain {
private:
	MT_Shader *m_shader;
	MT_HeightMap *m_heightMap;
	//FLOAT m_heightMap[k_HeightMapDepth][k_HeightMapWidth];
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	ID3D11InputLayout *m_layout; 
	void CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob);
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadIndexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void RenderFrame(ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
};

#endif