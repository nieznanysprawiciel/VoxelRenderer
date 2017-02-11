#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"


struct OctreeNode
{
	uint32		ChildPackPtr	: 23;		///< Offset to first child in children array.
	uint32		IndirectPtr		: 1;		///< ChildPackPtr points to poiinter instead directly to children.
	uint32		ChildMask		: 8;		///< Says about children existance. All bits set to 0 means, that ChildPackPtr points to VoxelAttributes.
};

