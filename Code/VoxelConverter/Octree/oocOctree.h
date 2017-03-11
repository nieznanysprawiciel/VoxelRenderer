#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"


namespace ooc
{

/**@brief Source octree node type.*/
struct Octree
{
	uint64			ChildrenBaseOffset;
	int8			ChildOffset[ 8 ];
	uint64			DataAddress;
};


}	// ooc
