#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>

#include "DXMathOperator.h"


namespace vr
{


const uint8	NUM_WEIGHTS = 4;

// ================================ //
//
struct ShellMeshVertex
{
	DirectX::XMFLOAT3			Position;
	uint8						WeightIdx[ NUM_WEIGHTS ];
	float						Weights[ NUM_WEIGHTS ];
	DirectX::XMFLOAT3			Normal;
};


// ================================ //
//
inline bool		operator<		( const ShellMeshVertex& vertex1, const ShellMeshVertex& vertex2 )
{
	if( vertex1.Position < vertex2.Position )
		return true;
	if( vertex1.Position == vertex2.Position )
	{
		if( vertex1.Normal < vertex2.Normal )
			return true;
		if( vertex1.Normal == vertex2.Normal )
		{
			for( int i = 0; i < 4; ++i )
			{
				// First compare matrix indicies.
				if( vertex1.WeightIdx[ i ] < vertex2.WeightIdx[ i ] )
					return true;
				else if( vertex1.WeightIdx[ i ] > vertex2.WeightIdx[ i ] )
					return false;
				else
				{
					// Then compare weights.
					if( vertex1.Weights[ i ] < vertex2.Weights[ i ] )
						return true;
					else if( vertex1.Weights[ i ] > vertex2.Weights[ i ] )
						return false;
					else
						continue;
				}
			}
		}
	}

	return false;
}

}

