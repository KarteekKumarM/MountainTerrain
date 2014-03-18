#ifndef MT_TERRAIN_H
#define MT_TERRAIN_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_Shader.h"
#include "MT_HeightMap.h"
#include "MT_Texture.h"

static const FLOAT k_SingleCellWidth = 0.2f;
static const FLOAT k_SingleCellDepth = 0.2f;

struct TerrainVertex 
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
};

class MT_Terrain 
{
private:
	static const LPCWSTR k_VertexShaderFileName;
	static const LPCWSTR k_PixelShaderFileName;

	MT_Shader *m_shader;
	MT_HeightMap *m_heightMap;
	MT_Texture *m_texture;

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	ID3D11InputLayout *m_layout; 
	void CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob);
	XMVECTOR GetNormalOfTheVertexCalculatedFromTheTwoTrianglesToItsBottomLeft( UINT i, UINT j );
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadIndexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void RenderFrame(ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
};

#endif