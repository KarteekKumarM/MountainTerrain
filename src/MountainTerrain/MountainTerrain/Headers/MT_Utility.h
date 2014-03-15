#include <DirectXMath.h>
using namespace DirectX;

#include <d3d11.h>

XMFLOAT3 XMFloat3Subtract( XMFLOAT3 a, XMFLOAT3 b )
{
	return XMFLOAT3( a.x - b.x, a.y - b.y, a.z - b.z);
}

XMFLOAT3 XMFloat3Cross( XMFLOAT3 a, XMFLOAT3 b )
{
	XMFLOAT3 result;
	result.x = (a.y * b.z) - (b.y*a.z);
	result.y = -1 * ((a.x * b.z) - (b.x*a.z));
	result.z = (a.x * b.y) - (b.x*a.z);
	return result;
}

XMFLOAT3 XMFloat3Multiply( FLOAT scalar, XMFLOAT3 vector )
{
	return XMFLOAT3( vector.x * scalar, vector.y * scalar, vector.z * scalar );
}

XMFLOAT3 XMFloat3Divide( FLOAT scalar, XMFLOAT3 vector )
{
	return XMFLOAT3( vector.x / scalar, vector.y / scalar, vector.z / scalar );
}

XMFLOAT3 XMFloat3Add( XMFLOAT3 a, XMFLOAT3 b )
{
	return XMFLOAT3( a.x + b.x, a.y + b.y, a.z + b.z );
}

XMFLOAT3 XMFloat3Average( XMFLOAT3 a, XMFLOAT3 b )
{
	return XMFloat3Multiply( 0.5f, XMFloat3Add( a, b ) );
}