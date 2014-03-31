#ifndef MT_INPUTHANDLER_H
#define MT_INPUTHANDLER_H

#include <d3d11.h>

class MT_InputHandler 
{
private:
	bool m_forwardKeyDownFlag, m_backKeyDownFlag, m_leftKeyDownFlag, m_rightKeyDownFlag, m_upKeyDownFlag, m_downKeyDownFlag;
	bool m_lightToggleKeyDownFlag, m_textureToggleKeyDownFlag, m_wireMeshToggleKeyDownFlag;
	bool m_screenGrabKeyDownFlag;
public:
	void Init();

	bool IsForwardKeyPressed();
	bool IsBackKeyPressed();
	bool IsLeftKeyPressed();
	bool IsRightKeyPressed();
	bool IsUpKeyPressed();
	bool IsDownKeyPressed();
	bool IsScreenGrabKeyPressed();
	bool IsLightToggleKeyPressed();
	bool IsTextureToggleKeyPressed();
	bool IsWireMeshToggleKeyPressed();
	void ResetScreenGrabKey();

	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void Clean();
};

#endif