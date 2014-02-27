#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class MT_Shader {
private:
	ID3D11VertexShader *m_d3dVertexShader;
	ID3D11PixelShader *m_d3dPixelShader;
	ID3DBlob *m_vertexShaderBlob;
	ID3DBlob *m_pixelShaderBlob;
public:
	HRESULT Init(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext);
	void Clean();
	ID3DBlob* GetVertexShaderBlob() { return m_vertexShaderBlob; }
	ID3DBlob* GetPixelShaderBlob() { return m_pixelShaderBlob; }

	static HRESULT CompileShaderFromFile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
};