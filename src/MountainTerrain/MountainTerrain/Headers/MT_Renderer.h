#ifndef MT_RENDERER_H
#define MT_RENDERER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "MT_ConstantBuffer.h"
#include "MT_Scene.h"

class MT_Renderer {
private:
	IDXGISwapChain *m_dxgiSwapChain;
	ID3D11Device *m_d3dDevice;
	ID3D11DeviceContext *m_d3dDeviceContext;
	ID3D11RenderTargetView *m_d3dBackBuffer;

	MT_ConstantBuffer *m_constantBuffer;
	MT_Scene *m_scene;
public:
	void Init(HWND hWnd, UINT screenWidth, UINT screenHeight);
	void InitConstantBuffer();
	void UpdateConstantBuffer();
	void RenderFrame();
	void Clean();
};

#endif