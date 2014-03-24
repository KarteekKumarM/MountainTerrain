#include "MT_Camera.h"

/**

Pitch:
- Rotate the camera around the right vector
- use to look up and down
Yaw
- Rotate the canera arount the up vector
- Use to look left and right
Roll
- Rotate around the look_At vector
- Of no use really 

**/

const float k_MovementSpeed = 0.02f;

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

void MT_Camera::ProcessInput(MT_InputHandler *inputHandler, float dTime) 
{
	// move forward or back
	if(inputHandler->IsForwardKeyPressed()) {
		MoveForward(dTime * k_MovementSpeed);
	} else if(inputHandler->IsBackKeyPressed()) {
		MoveBack(dTime * k_MovementSpeed);
	}

	// turn left or right
	if(inputHandler->IsLeftKeyPressed()) {
		TurnLeft(dTime * k_MovementSpeed);
	} else if(inputHandler->IsRightKeyPressed()) {
		TurnRight(dTime * k_MovementSpeed);
	}

	// turn up or down
	if(inputHandler->IsUpKeyPressed()) {
		TurnUp(dTime * k_MovementSpeed);
	} else if(inputHandler->IsDownKeyPressed()) {
		TurnDown(dTime * k_MovementSpeed);
	}
}

void MT_Camera::Clean() 
{

}