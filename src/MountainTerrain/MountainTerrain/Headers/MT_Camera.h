#ifndef MT_CAMERA_H
#define MT_CAMERA_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_InputHandler.h"

class MT_Camera 
{
private:
	XMVECTOR m_eye, m_lookat, m_up;
public:
	void Init();
	XMVECTOR GetEye();
	XMMATRIX GetViewMatrix();
	void MoveForward(FLOAT distance);
	void MoveBack(FLOAT distance);
	void TurnUp(FLOAT angle);
	void TurnDown(FLOAT angle);
	void TurnLeft(FLOAT angle);
	void TurnRight(FLOAT angle);
	void ProcessInput(MT_InputHandler *inputHandler);
	bool WillBeOutOfBounds(XMVECTOR vector);
	void Clean();
};

#endif