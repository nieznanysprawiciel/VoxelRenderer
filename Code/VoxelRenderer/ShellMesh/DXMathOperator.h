#pragma once

#include <DirectXMath.h>



//====================================================================================//
//			Comparision operators	
//====================================================================================//

namespace DirectX
{


// ================================ //
//
inline bool		operator==( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 )
{
	if( vertex1.x == vertex2.x &&
		vertex1.y == vertex2.y )
		return true;
	return false;
}

// ================================ //
//
inline bool		operator==( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 )
{
	if( vertex1.x == vertex2.x &&
		vertex1.y == vertex2.y &&
		vertex1.z == vertex2.z )
		return true;
	return false;
}

// ================================ //
//
inline bool		operator<( const DirectX::XMFLOAT2& vertex1, const DirectX::XMFLOAT2& vertex2 )
{
	if( vertex1.x < vertex2.x )
		return true;
	if( vertex1.x > vertex2.x )
		return false;

	if( vertex1.y < vertex2.y )
		return true;
	return false;
}

// ================================ //
//
inline bool		operator<( const DirectX::XMFLOAT3& vertex1, const DirectX::XMFLOAT3& vertex2 )
{
	if( vertex1.x < vertex2.x )
		return true;
	if( vertex1.x > vertex2.x )
		return false;

	if( vertex1.y < vertex2.y )
		return true;
	if( vertex1.y > vertex2.y )
		return false;

	if( vertex1.z < vertex2.z )
		return true;
	return false;
}

}	// DirectX
