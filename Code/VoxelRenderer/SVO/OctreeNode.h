#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"



/**@brief Represents normal node with children.*/
struct OctreeNode
{
	uint32		ChildPackPtr	: 23;		///< Offset to first child in children array.
	uint32		IndirectPtr		: 1;		///< ChildPackPtr points to poiinter instead directly to children.
	uint32		ChildMask		: 8;		///< Says about children existance. All bits set to 0 means, that ChildPackPtr points to VoxelAttributes.
};


/**@brief Represents offset to child node.

It is used if 23 bits of OctreeNode::ChildPackPtr is not enough to index next node.
Normally OctreeNodes are enough, so this struct should be used seldom.*/
struct OctreeFarPointer
{
	uint32		Offset;						///< Offset to next children array.
};
