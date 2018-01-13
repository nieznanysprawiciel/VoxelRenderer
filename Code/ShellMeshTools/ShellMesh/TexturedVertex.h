#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>

#include "DXMathOperator.h"


namespace vr
{


// ================================ //
//
struct TexturedVertex
{
	DirectX::XMFLOAT3			Position;
	DirectX::XMFLOAT3			Normal;
	DirectX::XMFLOAT2			UV;
	float						MaterialID;
};


// ================================ //
//
inline bool		operator<		( const TexturedVertex& vertex1, const TexturedVertex& vertex2 )
{
	if( vertex1.Position < vertex2.Position )
		return true;
	if( vertex1.Position == vertex2.Position )
	{
		if( vertex1.Normal < vertex2.Normal )
			return true;
		if( vertex1.Normal == vertex2.Normal )
		{
			if( vertex1.UV < vertex2.UV )
				return true;
			if( vertex1.UV == vertex2.UV )
			{
				if( vertex1.MaterialID < vertex2.MaterialID )
					return true;
			}
		}
	}

	return false;
}



}	// vr


