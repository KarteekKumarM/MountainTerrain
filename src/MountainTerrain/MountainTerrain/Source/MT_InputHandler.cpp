#include "MT_InputHandler.h"

static const char k_ForwardKey = 'w';
static const char k_BackKey = 's';
static const char k_LeftKey = 'a';
static const char k_RightKey = 'd';
static const char k_UpKey = 'e';
static const char k_DownKey = 'c';

void MT_InputHandler::Init() 
{
	forwardKeyDownFlag = false;
	backKeyDownFlag = false;
}

void MT_InputHandler::Clean()
{

}

bool MT_InputHandler::isForwardKeyPressed() 
{
	return forwardKeyDownFlag;
}

bool MT_InputHandler::isBackKeyPressed()
{
	return backKeyDownFlag;
}

bool MT_InputHandler::isLeftKeyPressed()
{
	return leftKeyDownFlag;
}

bool MT_InputHandler::isRightKeyPressed()
{
	return rightKeyDownFlag;
}

bool MT_InputHandler::isUpKeyPressed()
{
	return upKeyDownFlag;
}

bool MT_InputHandler::isDownKeyPressed() 
{
	return downKeyDownFlag;
}

bool MT_InputHandler::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	// char
	bool stateChanged = false;
	char keyChar = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
	if(keyChar != '\0') {

		// ignore case
		keyChar = tolower(keyChar);

		// key down or key up
		bool isKeyDown = message == WM_KEYDOWN;
		if(message == WM_KEYUP) {
			isKeyDown = false;
		}

		// update the correct key flag
		stateChanged = true;
		switch(keyChar) {
		case k_ForwardKey:
			forwardKeyDownFlag = isKeyDown;
			break;
		case k_BackKey:
			backKeyDownFlag = isKeyDown;
			break;
		case k_LeftKey:
			leftKeyDownFlag = isKeyDown;
			break;
		case k_RightKey:
			rightKeyDownFlag = isKeyDown;
			break;
		case k_UpKey:
			upKeyDownFlag = isKeyDown;
			break;
		case k_DownKey:
			downKeyDownFlag = isKeyDown;
			break;
		default:
			// none of the cases matched
			stateChanged = false;
		}
	}
	return stateChanged;
}