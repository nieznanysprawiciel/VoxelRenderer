#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"


namespace vr
{


/**@brief Represents normal node with children.*/
struct OctreeNode
{
	uint32		IsLeafNode : 1;			///< If this field is set to 1, you should use OctreeLeaf struct instead.
	uint32		IndirectPtr : 1;		///< ChildPackPtr points to poiinter instead directly to children.
	uint32		ChildPackPtr : 22;		///< Offset to first child in children array.
	uint32		ChildMask : 8;			///< Says about children existance. All bits set to 0 means, that ChildPackPtr points to VoxelAttributes.
};


/**@brief Represents leaf node with attributes.

@note We need more place for addressing attributes. That's why this struct exists.*/
struct OctreeLeaf
{
	uint32		IsLeafNode : 1;			///< Tells it's leaf node. Compatibility with OctreeNode struct.
	uint32		AttributesOffset : 31;	///< Offset of attribute structure realtive to Attributes data pool beginning.
										///< @note It's not offset from this node like in OctreeNode structure.
};


/**@brief Represents offset to child node.

It is used if 23 bits of OctreeNode::ChildPackPtr is not enough to index next node.
Normally OctreeNodes are enough, so this struct should be used seldom.*/
struct OctreeFarPointer
{
	uint32		Offset;						///< Offset to next children array.
};


/**@brief Structure at the beginning of data.*/
struct BlockDescriptor
{
	uint32			AttributesOffset;		///< Offset to beginning of attributes block.
	uint32			RootNodeOffset;			///< Offset to root node of nodes hierarchy.
};

}	// svo
