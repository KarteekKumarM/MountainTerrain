#ifndef MT_SCENE_H
#define MT_SCENE_H

#include "MT_Shader.h"

#include <DirectXMath.h>
using namespace DirectX;

struct Vertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

class MT_Scene {
private:
	MT_Shader *m_shader;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	ID3D11InputLayout *m_layout; 

	void LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void LoadIndexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);

	void CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob);
public:
	void Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void RenderFrame(ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
};

#endif