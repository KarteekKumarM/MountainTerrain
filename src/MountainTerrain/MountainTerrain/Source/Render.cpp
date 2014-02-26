#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

// globals
ID3D11Buffer *vertexBuffer;
ID3D11InputLayout *layout; 

struct Vertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

void LoadVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext) {
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.45f, -0.5, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	};

	// create the vertex buffer desc
	D3D11_BUFFER_DESC vertexBuffDesc;
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
    vertexBuffDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    vertexBuffDesc.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
    vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	d3dDevice->CreateBuffer(&vertexBuffDesc, NULL, &vertexBuffer);

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    d3dDeviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, vertices, sizeof(vertices));									  // copy the data
    d3dDeviceContext->Unmap(vertexBuffer, NULL);                                      // unmap the buffer
}

void CreateInputLayoutObjectForVertexBuffer(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, ID3DBlob *vertexShaderBlob) {
	// create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    d3dDevice->CreateInputLayout(ied, 2, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &layout);
    d3dDeviceContext->IASetInputLayout(layout);
}

void DrawFunc(ID3D11DeviceContext *d3dDeviceContext) {

	// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// select which primtive type we are using
	d3dDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	d3dDeviceContext->Draw(3, 0);

}

void CleanRender() {
	if(vertexBuffer) vertexBuffer->Release();
	if(layout) layout->Release();
}