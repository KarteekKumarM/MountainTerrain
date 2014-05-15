#include "MT_InputHandler.h"
#include "MT_Settings.h"

static const char k_ForwardKey = KEY_FORWARD;
static const char k_BackKey = KEY_BACK;
static const char k_LeftKey = KEY_LEFT;
static const char k_RightKey = KEY_RIGHT;
static const char k_UpKey = KEY_UP;
static const char k_DownKey = KEY_DOWN;
static const char k_ScreenGrabKey = KEY_SCREEN_SHOT;
static const char k_LightToggleKey = KEY_TOGGLE_LIGHT;
static const char k_TextureToggleKey = KEY_TOGGLE_TEXTURE;
static const char k_WireMeshToggleKey = KEY_TOGGLE_WIREMESH;
static const char k_SkyboxToggleKey = KEY_TOGGLE_SKYBOX;
static const char k_TerrainToggleKey = KEY_TOGGLE_TERRAIN;

#define BUTTON_FORWARD					(1<<0)
#define BUTTON_BACK						(1<<1)
#define BUTTON_LEFT						(1<<2)
#define BUTTON_RIGHT					(1<<3)
#define BUTTON_UP						(1<<4)
#define BUTTON_DOWN						(1<<5)
#define BUTTON_LIGHT_TOGGLE				(1<<6)
#define BUTTON_TEXTURE_TOGGLE			(1<<7)
#define BUTTON_WIREMESH_TOGGLE			(1<<8)
#define BUTTON_SCREEN_GRAB				(1<<9)
#define BUTTON_SKYBOX_TOGGLE			(1<<10)
#define BUTTON_TERRAIN_TOGGLE			(1<<11)

void MT_InputHandler::Init() 
{
	
}

void MT_InputHandler::Clean()
{

}

bool MT_InputHandler::IsTerrainToggleKeyPressed()
{
	return (m_buttons & BUTTON_TERRAIN_TOGGLE) != 0;
}

bool MT_InputHandler::IsSkyToggleKeyPressed()
{
	return (m_buttons & BUTTON_SKYBOX_TOGGLE) != 0;
}

bool MT_InputHandler::IsForwardKeyPressed() 
{
	return (m_buttons & BUTTON_FORWARD) != 0;
}

bool MT_InputHandler::IsBackKeyPressed()
{
	return (m_buttons & BUTTON_BACK) != 0;
}

bool MT_InputHandler::IsLeftKeyPressed()
{
	return (m_buttons & BUTTON_LEFT) != 0;
}

bool MT_InputHandler::IsRightKeyPressed()
{
	return (m_buttons & BUTTON_RIGHT) != 0;
}

bool MT_InputHandler::IsUpKeyPressed()
{
	return (m_buttons & BUTTON_UP) != 0;
}

bool MT_InputHandler::IsDownKeyPressed() 
{
	return (m_buttons & BUTTON_DOWN) != 0;
}

bool MT_InputHandler::IsScreenGrabKeyPressed()
{
	return (m_buttons & BUTTON_SCREEN_GRAB) != 0;
}

bool MT_InputHandler::IsLightToggleKeyPressed()
{
	return (m_buttons & BUTTON_LIGHT_TOGGLE) != 0;
}

bool MT_InputHandler::IsTextureToggleKeyPressed()
{
	return (m_buttons & BUTTON_TEXTURE_TOGGLE) != 0;
}

bool MT_InputHandler::IsWireMeshToggleKeyPressed()
{
	return (m_buttons & BUTTON_WIREMESH_TOGGLE) != 0;
}

void MT_InputHandler::ResetScreenGrabKey()
{
	m_buttons &= BUTTON_SCREEN_GRAB;
}

void MT_InputHandler::SetButton(bool setFlag, int flagToSet)
{
	if (setFlag)
	{
		m_buttons |= flagToSet;
	}
	else
	{
		m_buttons &= !flagToSet;
	}
}

bool MT_InputHandler::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	// char
	bool stateChanged = false;
	char keyChar = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
	if(keyChar != '\0') {

		keyChar = tolower(keyChar);

		bool isKeyDown = message == WM_KEYDOWN;
		if(message == WM_KEYUP) {
			isKeyDown = false;
		}

		// only one key input affects at a time in this order
		stateChanged = true;
		switch(keyChar) {
		case k_ForwardKey:
			SetButton(isKeyDown, BUTTON_FORWARD);
			break;
		case k_BackKey:
			SetButton(isKeyDown, BUTTON_BACK);
			break;
		case k_LeftKey:
			SetButton(isKeyDown, BUTTON_LEFT);
			break;
		case k_RightKey:
			SetButton(isKeyDown, BUTTON_RIGHT);
			break;
		case k_UpKey:
			SetButton(isKeyDown, BUTTON_UP);
			break;
		case k_DownKey:
			SetButton(isKeyDown, BUTTON_DOWN);
			break;
		case k_ScreenGrabKey:
			SetButton(isKeyDown, BUTTON_SCREEN_GRAB);
			break;
		case k_LightToggleKey:
			SetButton(isKeyDown, BUTTON_LIGHT_TOGGLE);
			break;
		case k_TextureToggleKey:
			SetButton(isKeyDown, BUTTON_TEXTURE_TOGGLE);
			break;
		case k_WireMeshToggleKey:
			SetButton(isKeyDown, BUTTON_WIREMESH_TOGGLE);
			break;
		case k_SkyboxToggleKey:
			SetButton(isKeyDown, BUTTON_SKYBOX_TOGGLE);
			break;
		case k_TerrainToggleKey:
			SetButton(isKeyDown, BUTTON_TERRAIN_TOGGLE);
			break;
		default:
			stateChanged = false;
		}
	}
	return stateChanged;
}