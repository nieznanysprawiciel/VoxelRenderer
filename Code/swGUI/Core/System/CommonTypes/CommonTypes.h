#pragma once
/**
@file CommonTypes.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include <string>
#include <DirectXMath.h>

#include "Thickness.h"




namespace sw {
namespace gui
{



static const std::string	EMPTY_STRING;


typedef DirectX::XMFLOAT2	Position;
typedef DirectX::XMFLOAT2	Size2D;


/**@brief Rectangle structure.*/
struct Rect
{
	float			Left;
	float			Right;
	float			Top;
	float			Bottom;

	float		GetWidth	() { return Right - Left; }
	float		GetHeight	() { return Top - Bottom; }

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

}	// gui
}	// sw
