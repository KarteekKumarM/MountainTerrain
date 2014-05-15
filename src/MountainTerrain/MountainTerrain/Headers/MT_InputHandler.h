#ifndef MT_INPUTHANDLER_H
#define MT_INPUTHANDLER_H

#include <d3d11.h>

class MT_InputHandler 
{
private:
	int m_buttons;
	void SetButton(bool setFlag, int flagToSet);
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
	bool IsSkyToggleKeyPressed();
	bool IsTerrainToggleKeyPressed();

	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void Clean();
};

#endif