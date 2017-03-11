#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>


namespace ooc
{

/**@brief Source octree data format;*/
struct Payload
{
	uint64					MortonCode;
	DirectX::XMFLOAT3		Color;
	DirectX::XMFLOAT3		Normal;
};


}	// ooc

