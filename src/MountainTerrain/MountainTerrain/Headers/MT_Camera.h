#ifndef MT_CAMERA_H
#define MT_CAMERA_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "MT_InputHandler.h"

class MT_Camera 
{
private:
	XMVECTOR eye, lookat, up;
public:
	void Init();
	XMMATRIX GetViewMatrix();
	void MoveForward(FLOAT distance);
	void MoveBack(FLOAT distance);
	void TurnUp(FLOAT angle);
	void TurnDown(FLOAT angle);
	void TurnLeft(FLOAT angle);
	void TurnRight(FLOAT angle);
	void ProcessInput(MT_InputHandler *inputHandler, float dTime);
	void Clean();
};

#endif