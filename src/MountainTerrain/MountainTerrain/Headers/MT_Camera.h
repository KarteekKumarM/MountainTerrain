#ifndef MT_CAMERA_H
#define MT_CAMERA_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

class MT_Camera {
private:
	XMVECTOR eye, lookat, up;
public:
	void Init();
	XMMATRIX GetViewMatrix();
	void MoveForward(FLOAT distance);
	void MoveBack(FLOAT distance);
	void Clean();
};

#endif