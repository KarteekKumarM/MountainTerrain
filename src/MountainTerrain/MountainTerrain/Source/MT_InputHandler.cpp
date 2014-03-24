#include "MT_InputHandler.h"

static const char k_ForwardKey = 'w';
static const char k_BackKey = 's';
static const char k_LeftKey = 'a';
static const char k_RightKey = 'd';
static const char k_UpKey = 'e';
static const char k_DownKey = 'c';
static const char k_ScreenGrabKey = 'k';

void MT_InputHandler::Init() 
{
	m_forwardKeyDownFlag = false;
	m_backKeyDownFlag = false;
}

void MT_InputHandler::Clean()
{

}

bool MT_InputHandler::IsForwardKeyPressed() 
{
	return m_forwardKeyDownFlag;
}

bool MT_InputHandler::IsBackKeyPressed()
{
	return m_backKeyDownFlag;
}

bool MT_InputHandler::IsLeftKeyPressed()
{
	return m_leftKeyDownFlag;
}

bool MT_InputHandler::IsRightKeyPressed()
{
	return m_rightKeyDownFlag;
}

bool MT_InputHandler::IsUpKeyPressed()
{
	return m_upKeyDownFlag;
}

bool MT_InputHandler::IsDownKeyPressed() 
{
	return m_downKeyDownFlag;
}

bool MT_InputHandler::IsScreenGrabKeyPressed()
{
	return m_screenGrabKeyDownFlag;
}

void MT_InputHandler::ResetScreenGrabKey()
{
	m_screenGrabKeyDownFlag = false;
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
			m_forwardKeyDownFlag = isKeyDown;
			break;
		case k_BackKey:
			m_backKeyDownFlag = isKeyDown;
			break;
		case k_LeftKey:
			m_leftKeyDownFlag = isKeyDown;
			break;
		case k_RightKey:
			m_rightKeyDownFlag = isKeyDown;
			break;
		case k_UpKey:
			m_upKeyDownFlag = isKeyDown;
			break;
		case k_DownKey:
			m_downKeyDownFlag = isKeyDown;
			break;
		case k_ScreenGrabKey:
			m_screenGrabKeyDownFlag = isKeyDown;
			break;
		default:
			// none of the cases matched
			stateChanged = false;
		}
	}
	return stateChanged;
}