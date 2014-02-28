#include "MT_Camera.h"

void MT_Camera::Init() {
	eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	lookat = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
}

void MT_Camera::MoveForward(FLOAT distance) {
	XMVECTOR viewDirection = lookat - eye;
	viewDirection = distance * XMVector3Normalize(viewDirection);
	
	eye += viewDirection;
	lookat += viewDirection;
}

void MT_Camera::MoveBack(FLOAT distance) {
	MoveForward(-1 * distance);
}

XMMATRIX MT_Camera::GetViewMatrix() {
	return DirectX::XMMatrixLookAtLH(eye, lookat, up);
}

void MT_Camera::Clean() {

}