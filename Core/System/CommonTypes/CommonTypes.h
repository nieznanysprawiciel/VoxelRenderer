#pragma once

#include <string>
#include <DirectXMath.h>

#include "Thickness.h"

namespace GUI
{

static const std::string	EMPTY_STRING;


typedef DirectX::XMFLOAT2	Position;


struct Rect
{
	float	Left;
	float	Right;
	float	Top;
	float	Bottom;

	float		GetWidth	()		{ return Right - Left; }
	float		GetHeight	()		{ return Top - Bottom; }

	bool		Intersects	( const Position& pos )
	{
		if( pos.x < Left )
			return false;
		if( pos.x > Right )
			return false;
		if( pos.y < Bottom )
			return false;
		if( pos.y > Top )
			return false;
		return true;
	}
};

}	// GUI
