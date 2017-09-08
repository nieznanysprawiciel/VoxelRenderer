#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"


namespace vr
{


namespace impl
{
	constexpr uint32 ChildPtrPackMask = ~( 0xFF | 0xC0000000 );
	constexpr uint32 LeafBitMask = ( 0x1 << 31 );
	constexpr uint32 IndirectBitMask = ( 0x1 << 30 );
	constexpr uint32 ChildBitsMask = 0xFF;

	constexpr uint32 AttribOffsetMask = 0x7FFFFFFF;
}



/**@brief Represents normal node with children.

Structure layout look like this:
uint32		IsLeafNode : 1;
uint32		IsIndirectPtr : 1;
uint32		ChildPackPtr : 22;	
uint32		ChildMask : 8;
*/
struct OctreeNode
{
	uint32		Data;

// ================================ //
//
	///< If this field is set to 1, you should use OctreeLeaf struct instead.
	bool		IsLeafNode		() const { return ( Data & impl::LeafBitMask ) != 0; }

	///< ChildPackPtr points to pointer instead directly to children.
	bool		IsIndirectPtr	() const { return ( Data & impl::IndirectBitMask ) != 0; }

	///< Says about children existance. All bits set to 0 means, that ChildPackPtr points to VoxelAttributes.
	uint8		ChildMask		() const { return Data & impl::ChildBitsMask;  }

	///< Offset to first child in children array.
	uint32		ChildPackPtr	() const { return ( Data & impl::ChildPtrPackMask ) >> 8; }

// ================================ //
//
	void		SetIsLeafNode		( bool isLeaf )		{ Data = ( isLeaf ? Data | impl::LeafBitMask : Data ); }
	void		SetIsIndirectPtr	( bool isIndirect ) { Data = ( isIndirect ? Data | impl::IndirectBitMask : Data ); }
	void		SetChildPackPtr		( uint32 ptrPack )	{ Data = ( Data & ~impl::ChildPtrPackMask ) | ( impl::ChildPtrPackMask & ( ptrPack << 8 ) ); }
	void		SetChildMask		( uint8 childMask ) { Data = ( Data & ~impl::ChildBitsMask ) | ( impl::ChildBitsMask & childMask ); }
};


/**@brief Represents leaf node with attributes.

uint32		IsLeafNode : 1;
uint32		AttributesOffset : 31;

@note We need more place for addressing attributes. That's why this struct exists.*/
struct OctreeLeaf
{
	uint32		Data;

// ================================ //
//
	///< Tells it's leaf node. Compatibility with OctreeNode struct.
	bool		IsLeafNode			() const { return ( Data & impl::LeafBitMask ) != 0; }

	///< Offset of attribute structure realtive to Attributes data pool beginning.
	///< @note It's not offset from this node like in OctreeNode structure.
	uint32		AttributesOffset	() const { return Data & impl::AttribOffsetMask; }

// ================================ //
//
	void		SetIsLeafNode		( bool isLeaf )			{ Data = ( isLeaf ? Data | impl::LeafBitMask : Data ); }
	void		SetAttributeOffset	( uint32 attribute )	{ Data = ( Data & ~impl::AttribOffsetMask ) | ( attribute & impl::AttribOffsetMask ); }
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

template< typename CastType >
CastType&			Cast		( OctreeNode& node )
{
	return reinterpret_cast< CastType& >( node );
}

template< typename CastType >
const CastType&		Cast		( const OctreeNode& node )
{
	return reinterpret_cast< const CastType& >( node );
}

}	// svo
