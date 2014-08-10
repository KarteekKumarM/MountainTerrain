#ifndef MT_UTILITY_H
#define MT_UTILITY_H

#include <DirectXMath.h>
using namespace DirectX;

#include <d3d11.h>

void GetTimeString(char **timeResult);

XMFLOAT3 XMFloat3Subtract(XMFLOAT3 a, XMFLOAT3 b);

XMFLOAT3 XMFloat3Cross(XMFLOAT3 a, XMFLOAT3 b);

XMFLOAT3 XMFloat3Multiply(FLOAT scalar, XMFLOAT3 vector);

XMFLOAT3 XMFloat3Divide(FLOAT scalar, XMFLOAT3 vector);

XMFLOAT3 XMFloat3Add(XMFLOAT3 a, XMFLOAT3 b);

XMFLOAT3 XMFloat3Average(XMFLOAT3 a, XMFLOAT3 b);

float XMFloat3Dot(XMFLOAT3 a, XMFLOAT3 b);

float XMFloat3Distance(XMFLOAT3 a, XMFLOAT3 b);

#endif