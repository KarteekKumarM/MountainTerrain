#include "MT_Camera.h"
#include "MT_Logger.h"

/**

Move:
- Move along the look at vector
- (Keys: w/s)

Pitch:
- Rotate the camera around the right vector
- use to look up and down (Mouse: move up/down, Keys: Up/Down)
Yaw
- Rotate the canera arount the up vector
- Use to look left and right (Mouse: move left/right. Keys: Left/Right)
Roll
- Rotate around the look_At vector
- Of no use really - 

**/

void MT_Camera::Init() 
{
	// TO DO, script eye and view direction - then calculate look at and up
	eye = XMVectorSet(0.0f, 20.0f, 0.0f, 0.0f);
	lookat = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
}

void MT_Camera::MoveForward(FLOAT distance) 
{
	XMVECTOR viewDirection = lookat - eye;
	viewDirection = distance * XMVector3Normalize(viewDirection);
	
	eye += viewDirection;
	lookat += viewDirection;
}

void MT_Camera::MoveBack(FLOAT distance) 
{
	MoveForward(-1 * distance);
}

XMVECTOR RotateVectorByAngleAroundAxis(XMVECTOR inputVector, XMVECTOR rotationAxis, FLOAT angle) 
{
	XMVECTOR projectionVector = XMVector3Dot(inputVector, rotationAxis) * rotationAxis;
	XMVECTOR rejectionVector = inputVector - projectionVector;

	// calculate new rejection vector
	XMVECTOR outwardVector = XMVector3Cross(rotationAxis, inputVector);
	XMVECTOR rejectionVectorAfterRotation = rejectionVector * cos(angle) + outwardVector * sin(angle);

	XMVECTOR outputVector = projectionVector + rejectionVectorAfterRotation;

	return outputVector;
}

void MT_Camera::TurnUp(FLOAT angle)
{
	XMVECTOR viewDirection = lookat - eye;
	XMVECTOR length = XMVector3Length(viewDirection);
	viewDirection = XMVector3Normalize(viewDirection);

	XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(viewDirection, up));
	
	up = XMVector3Normalize(up);

	viewDirection = viewDirection * cos(angle) + up * sin(angle);
	viewDirection = XMVector3Normalize(viewDirection);
	
	up = XMVector3Normalize(XMVector3Cross(rightVector, viewDirection));

	lookat = eye + (viewDirection * length);
}
void MT_Camera::TurnDown(FLOAT angle)
{
	TurnUp(-1 * angle);
}

void MT_Camera::TurnRight(FLOAT angle) 
{
	TurnLeft(-1 * angle);
}

void MT_Camera::TurnLeft(FLOAT angle) 
{
	XMVECTOR viewDirection = lookat - eye;

	XMVECTOR length = XMVector3Length(viewDirection);
	viewDirection = XMVector3Normalize(viewDirection);

	XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(viewDirection, up));

	up = XMVector3Normalize(up);

	viewDirection = viewDirection * cos(angle) + rightVector * sin(angle);
	viewDirection = XMVector3Normalize(viewDirection);

	lookat = eye + (viewDirection * length);
}

XMMATRIX MT_Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(eye, lookat, up);
}

void MT_Camera::ProcessInput(MT_InputHandler *inputHandler) 
{

	float k_angleTurnPerUnitTime = 10 * 0.0174532925; // speed of turn - converted to radian
	float k_distMovePerUnitTime = 1.0f; // speed of move
	float dTime = 0.5;

	// move forward or back
	if(inputHandler->IsForwardKeyPressed()) {
		MoveForward(dTime * k_distMovePerUnitTime);
	} else if(inputHandler->IsBackKeyPressed()) {
		MoveBack(dTime * k_distMovePerUnitTime);
	}

	// turn left or right
	if(inputHandler->IsLeftKeyPressed()) {
		TurnLeft(dTime * k_angleTurnPerUnitTime);
	} else if(inputHandler->IsRightKeyPressed()) {
		TurnRight(dTime * k_angleTurnPerUnitTime);
	}

	// turn up or down
	if(inputHandler->IsUpKeyPressed()) {
		TurnUp(dTime * k_angleTurnPerUnitTime);
	} else if(inputHandler->IsDownKeyPressed()) {
		TurnDown(dTime * k_angleTurnPerUnitTime);
	}
}

void MT_Camera::Clean() 
{
	
}