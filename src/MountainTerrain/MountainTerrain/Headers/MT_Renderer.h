#ifndef MT_RENDERER_H
#define MT_RENDERER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "MT_ConstantBuffer.h"
#include "MT_Terrain.h"
#include "MT_Skybox.h"

class MT_Renderer {
private:
	IDXGISwapChain *m_dxgiSwapChain;
	ID3D11Device *m_d3dDevice;
	ID3D11DeviceContext *m_d3dDeviceContext;
	ID3D11Texture2D *m_BackBuffer;
	ID3D11RenderTargetView *m_d3dBackBufferRenderTargetView;
	ID3D11RasterizerState *m_d3dRasterizerState;
	ID3D11Texture2D *m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11BlendState* m_alphaEnableBlendingState;

	MT_ConstantBuffer m_constantBuffer;
	MT_Skybox *m_skybox;
	MT_Terrain *m_terrain;
	MT_Camera *m_latestCameraState;

	bool m_wireFrameEnabled;
	bool m_skyboxToggle;
	bool m_terrainToggle;

	bool SetupRasterizer();
	void SetupViewPort(UINT screenWidth, UINT screenHeight);
	bool SetupBackBuffer();
	bool SetupDepthStencilBuffer(UINT screenWidth, UINT screenHeight);
	bool SetupBlending();
public:
	void Init(HWND hWnd, UINT screenWidth, UINT screenHeight);
	void ProcessInput(MT_InputHandler *inputHandler);
	void ProcessCameraState(MT_Camera *camera);
	void RenderFrame();
	void CaptureFrame();
	void Clean();
};

#endif