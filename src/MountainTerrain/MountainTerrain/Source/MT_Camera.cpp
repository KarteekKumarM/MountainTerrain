#include "MT_Camera.h"
#include "MT_Logger.h"
#include "MT_Settings.h"

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
	m_eye = XMVectorSet(CAMERA_EYE);
	m_lookat = XMVectorSet(CAMERA_LOOKAT);
	m_up = XMVectorSet(CAMERA_UP);
}

bool MT_Camera::WillBeOutOfBounds(XMVECTOR vector)
{
	XMFLOAT3 test_eye;
	XMStoreFloat3(&test_eye, m_eye + vector);

	if (fabs(test_eye.x) > 100.0f)
		return true;

	if (fabs(test_eye.z) > 100.0f)
		return true;

	if (fabs(test_eye.y) > 50.0f)
		return true;

	if (fabs(test_eye.y) < 10.0f)
		return true;

	return false;
}

void MT_Camera::MoveForward(FLOAT distance) 
{
	XMVECTOR viewDirection = m_lookat - m_eye;
	viewDirection = distance * XMVector3Normalize(viewDirection);

	if (WillBeOutOfBounds(viewDirection))
		return;
	
	m_eye += viewDirection;
	m_lookat += viewDirection;
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
	XMVECTOR viewDirection = m_lookat - m_eye;
	XMVECTOR length = XMVector3Length(viewDirection);
	viewDirection = XMVector3Normalize(viewDirection);

	XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(viewDirection, m_up));
	
	m_up = XMVector3Normalize(m_up);

	viewDirection = viewDirection * cos(angle) + m_up * sin(angle);
	viewDirection = XMVector3Normalize(viewDirection);
	
	m_up = XMVector3Normalize(XMVector3Cross(rightVector, viewDirection));

	m_lookat = m_eye + (viewDirection * length);
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
	XMVECTOR viewDirection = m_lookat - m_eye;

	XMVECTOR length = XMVector3Length(viewDirection);
	viewDirection = XMVector3Normalize(viewDirection);

	XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(viewDirection, m_up));

	m_up = XMVector3Normalize(m_up);

	viewDirection = viewDirection * cos(angle) + rightVector * sin(angle);
	viewDirection = XMVector3Normalize(viewDirection);

	m_lookat = m_eye + (viewDirection * length);
}

XMVECTOR MT_Camera::GetEye()
{
	return m_eye;
}

XMMATRIX MT_Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(m_eye, m_lookat, m_up);
}

void MT_Camera::ProcessInput(MT_InputHandler *inputHandler) 
{
	const float k_angleTurnPerUnitTime = ANGLE_TURN_PER_UNIT_TIME * MATH_DEGREE_TO_RADIAN; // speed of turn - converted to radian
	const float k_distMovePerUnitTime = DISTANCE_MOVE_PER_UNIT_TIME; // speed of move
	const float dTime = 0.5;

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