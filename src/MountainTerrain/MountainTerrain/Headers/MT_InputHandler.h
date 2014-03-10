#ifndef MT_INPUTHANDLER_H
#define MT_INPUTHANDLER_H

#include <d3d11.h>

class MT_InputHandler 
{
private:
	bool forwardKeyDownFlag, backKeyDownFlag, leftKeyDownFlag, rightKeyDownFlag, upKeyDownFlag, downKeyDownFlag;
public:
	void Init();

	bool isForwardKeyPressed();
	bool isBackKeyPressed();
	bool isLeftKeyPressed();
	bool isRightKeyPressed();
	bool isUpKeyPressed();
	bool isDownKeyPressed();

	bool ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void Clean();
};

#endif