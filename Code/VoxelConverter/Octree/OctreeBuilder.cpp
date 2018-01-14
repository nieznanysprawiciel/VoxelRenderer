#include "OctreeBuilder.h"

#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"

#include "VoxelRenderer/SVO/OctreeNode.h"
#include "VoxelRenderer/SVO/VoxelAttributes.h"

#include "VoxelConverter/Texturing/Texturing.h"
#include "VoxelConverter/Texturing/OctreeAccessor.h"

#include <fstream>



namespace vr
{


// ================================ //
//
bool				ReadNodes				( OctreeInfo& srcOctree, uint8* data )
{
	string nodesFile = srcOctree.base_filename + string( ".octreenodes" );
	Size size = srcOctree.n_nodes * sizeof( ooc::OctreeNode );

	//std::ifstream fileStream( nodesFile, std::ios_base::binary );
	FILE* file = fopen( nodesFile.c_str(), "rb" );
	if( file )
	{
		//fileStream.read( (char*)data, size );
		Size read = fread( (char*)data, 1, size, file );
		fclose( file );
		return true;
	}
	return false;
}

// ================================ //
//
bool				ReadData				( OctreeInfo& srcOctree, uint8* data )
{
	string dataFile = srcOctree.base_filename + string( ".octreedata" );
	Size size = srcOctree.n_data * sizeof( ooc::Payload );

	//std::ifstream fileStream( dataFile, std::ios_base::binary );
	FILE* file = fopen( dataFile.c_str(), "rb" );
	if( file )
	{
		//fileStream.read( (char*)data, size );
		Size read = fread( (char*)data, 1, size, file );
		fclose( file );
		return true;
	}
	return false;
}



// ================================ //
//
OctreeBuilder::OctreeBuilder()
	:	m_numInitIndirectPtrs( 512 )
	,	m_curNodesOffset( 0 )
	,	m_curIndirectOffset( 0 )
	,	m_attributesOffset( 0 )
	,	m_gridLength( 0 )
	,	m_maxDirectOffset( ComputeMaxDirectOffset() )
{}

// ================================ //
//
bool				OctreeBuilder::ReadOctree		( OctreeInfo& srcOctree )
{
	if( srcOctree.filesExist() )
	{
		m_numNodes = srcOctree.n_nodes;
		m_numAttribs = srcOctree.n_data;

		m_gridLength = srcOctree.gridlength;

		m_octree = std::unique_ptr< ooc::OctreeNode[] >( new ooc::OctreeNode[ m_numNodes ] );
		m_attributes = std::unique_ptr< ooc::Payload[] >( new ooc::Payload[ m_numAttribs ] );

		ReadNodes( srcOctree, (uint8*)m_octree.get() );
		ReadData( srcOctree, (uint8*)m_attributes.get() );

		return true;
	}

	return false;
}

// ================================ //
//
bool				OctreeBuilder::TextureOctree	( const filesystem::Path& filePath, SamplerType samplingType, bool flipU, bool flipV )
{
	OctreeAccessor octree( m_octree.get(), m_attributes.get(), m_numNodes, m_numAttribs );
	Texturing texturing( octree, flipU, flipV );

	return texturing.TextureOctree( filePath, samplingType );
}

// ================================ //
//
OctreePtr			OctreeBuilder::BuildOctree		()
{
	BuildEmptyStructure();
	BuildAttributesSegment();
	BuildNodeHierarchy();

	return std::make_shared< Octree >( std::move( m_data ), m_gridLength, m_curIndirectOffset );
}

// ================================ //
//
OctreePtr			OctreeBuilder::BuildOctree		( OctreeInfo& srcOctree )
{
	if( ReadOctree( srcOctree ) )
		return BuildOctree();

	return nullptr;
}

// ================================ //
//
void				OctreeBuilder::BuildEmptyStructure	()
{
	m_data.resize( ComputeMemory( m_numNodes, m_numAttribs ) / sizeof( OctreeNode ) );

	// Block descriptor + indirect pointers size.
	Size indirectSize = sizeof( BlockDescriptor ) + m_numInitIndirectPtrs * sizeof( vr::OctreeFarPointer );
	memset( m_data.data(), 0, indirectSize );
	
	// Fill block descriptor.
	BlockDescriptor& desc = Cast< BlockDescriptor& >( *m_data.data() );
	desc.RootNodeOffset = static_cast< uint32 >( indirectSize / sizeof( vr::OctreeNode ) );
	desc.AttributesOffset = static_cast< uint32 >( desc.RootNodeOffset + m_numNodes );

	m_curNodesOffset = desc.RootNodeOffset;
	m_curIndirectOffset = sizeof( BlockDescriptor ) / sizeof( vr::OctreeNode );
	m_attributesOffset = desc.AttributesOffset;
}

// ================================ //
//
void				OctreeBuilder::BuildAttributesSegment()
{
	for( Size i = 0; i < m_numAttribs; ++i )
	{
		ooc::Payload& payload = m_attributes[ i ];
		vr::VoxelAttributes& attribute = AccessAttributes( i );

		attribute.Normal = payload.Normal;
		attribute.Color = DirectX::PackedVector::XMCOLOR( payload.Color.x, payload.Color.y, payload.Color.z, 1.0f );
	}
}

// ================================ //
//
void				OctreeBuilder::BuildNodeHierarchy	()
{
	auto absolutOffset = m_curNodesOffset++;
	
	// Note: Last node is root node, not first.
	BuildNodeHierarchy( m_octree[ m_numNodes - 1 ], absolutOffset, Cast< vr::OctreeNode& >( m_data[ absolutOffset ] ) );
}

// ================================ //
//
void				OctreeBuilder::BuildNodeHierarchy	( ooc::OctreeNode& srcNode, Size srcOffset, vr::OctreeNode& dstNode )
{
	auto numChildren = CountChildren( srcNode );
	if( IsLeafNode( srcNode ) )
	{
		vr::OctreeLeaf& leaf = Cast< vr::OctreeLeaf& >( dstNode );
		leaf.SetIsLeafNode( true );
		leaf.SetAttributeOffset( ComputeAttribOffset( srcNode.DataAddress ) - (uint32)m_attributesOffset );
	}
	else
	{
		dstNode.SetIsLeafNode( false );
		SetChildMask( srcNode, dstNode );
		
		Size absolutOffset = AllocateNodes( numChildren );
		Size nodesOffset = absolutOffset - srcOffset;

		if( nodesOffset > m_maxDirectOffset )
		{
			// Make indirect pointer.
			auto farPtrOffset = AllocateIndirect();

			OctreeFarPointer& farPointer = AccessFarPtr( farPtrOffset );
			farPointer.Offset = (uint32)absolutOffset;

			dstNode.SetIsIndirectPtr( true );
			dstNode.SetChildPackPtr( farPtrOffset );
		}
		else
		{
			dstNode.SetIsIndirectPtr( false );
			dstNode.SetChildPackPtr( (uint32)nodesOffset );
		}


		const uint8 childMask = dstNode.ChildMask();
		const uint8 one = 1 << 7;
		uint8 childShift = 7;

		for( int i = 0; i < 8; ++i )
		{
			// If child exists.
			if( childMask & ( one >> childShift ) )
			{
				vr::OctreeNode& child = AccessNode( (uint32)absolutOffset );
				ooc::OctreeNode& srcChild = AccessNode( srcNode, InverseCoords( childShift ) );

				BuildNodeHierarchy( srcChild, absolutOffset, child );

				absolutOffset++;
			}

			childShift--;
		}
	}
}

// ================================ //
//
bool				OctreeBuilder::IsLeafNode			( ooc::OctreeNode& srcNode )
{
	bool hasChild = false;
	for( auto child : srcNode.ChildOffset )
	{
		if( child >= 0 )
			hasChild = true;
	}

	return !hasChild && srcNode.DataAddress != 0;
}

// ================================ //
//
int8				OctreeBuilder::CountChildren		( ooc::OctreeNode& srcNode )
{
	int8 numChildren = 0;
	for( auto child : srcNode.ChildOffset )
	{
		if( child >= 0 )
			numChildren++;
	}

	return numChildren;
}

// ================================ //
//
uint8				OctreeBuilder::InverseCoords		( uint8 childShift )
{
	// Negate bits and mask everything except last 3 bits.
	return ~childShift & 0x7;
}

// ================================ //
//
void				OctreeBuilder::SetChildMask			( ooc::OctreeNode& srcNode, vr::OctreeNode& dstNode )
{
	dstNode.SetChildMask( 0 );
	uint8 one = 1 << 7;	// Set highest bit to 1.

	for( uint8 i = 0; i < 8; ++i )
	{
		if( srcNode.ChildOffset[ i ] >= 0 )
		{
			// We use oposite coordinates system in raycaster.
			uint8 childShift = InverseCoords( i );
			dstNode.SetChildMask( dstNode.ChildMask() | ( one >> childShift ) );
		}
	}
}

// ================================ //
/// @param[in] dataOffset is index in m_attributes array.
VoxelAttributes&	OctreeBuilder::AccessAttributes		( uint64 dataOffset )
{
	auto offset = ComputeAttribOffset( dataOffset );
	return Cast< VoxelAttributes& >( m_data[ offset ] );
}

// ================================ //
//
vr::OctreeNode&		OctreeBuilder::AccessNode			( uint32 absolutOffset )
{
	return Cast< vr::OctreeNode& >( m_data[ absolutOffset ] );
}

// ================================ //
//
ooc::OctreeNode&	OctreeBuilder::AccessNode			( ooc::OctreeNode& parent, uint8 index )
{
	if( parent.ChildOffset[ index ] >= 0 )
	{
		Size offset = parent.ChildrenBaseOffset + parent.ChildOffset[ index ];
		return m_octree[ offset ];
	}

	assert( false );
	throw std::runtime_error( "Trying to access node after end of children array." );
	return ooc::OctreeNode();
}

// ================================ //
//
OctreeFarPointer&	OctreeBuilder::AccessFarPtr		( uint32 absolutOffset )
{
	return Cast< vr::OctreeFarPointer& >( m_data[ absolutOffset ] );
}

// ================================ //
//
Size				OctreeBuilder::AllocateNodes		( uint8 numNodes )
{
	Size firstNodeOffset = m_curNodesOffset;
	m_curNodesOffset += numNodes;
	return firstNodeOffset;
}

// ================================ //
//
uint32				OctreeBuilder::AllocateIndirect		()
{
	if( m_curIndirectOffset == m_numInitIndirectPtrs + sizeof( BlockDescriptor ) / sizeof( vr::OctreeNode ) )
		throw std::runtime_error( "To many indirect pointers" );

	auto absolutOffset = m_curIndirectOffset;
	m_curIndirectOffset += sizeof( vr::OctreeFarPointer ) / sizeof( vr::OctreeNode );
	return (uint32)absolutOffset;
}

// ================================ //
//
uint32				OctreeBuilder::ComputeAttribOffset	( uint64 dataOffset )
{
	// Data offset indexes m_attributes array. Sizeof( vr::OctreeNode ) is our allocation unit.
	// sizeof( vr::VoxelAttributes ) / sizeof( vr::OctreeNode ) is num allocations units possesed by VoxelAttributes.
	auto offset = m_attributesOffset + dataOffset * sizeof( vr::VoxelAttributes ) / sizeof( vr::OctreeNode );
	assert( offset < std::numeric_limits< uint32 >::max() );
	return (uint32)offset;
}

// ================================ //
//
ooc::OctreeNode&	OctreeBuilder::AccessNext			( ooc::OctreeNode& parent, int8& startIdx )
{
	for( ; startIdx >= 0; --startIdx )
	{
		if( parent.ChildOffset[ startIdx ] >= 0 )
		{
			Size offset = parent.ChildrenBaseOffset + parent.ChildOffset[ startIdx ];
			return m_octree[ offset ];
		}
	}

	assert( false );
	throw std::runtime_error( "Trying to access node after end of children array." );
	return ooc::OctreeNode();
}

// ================================ //
//
Size				OctreeBuilder::ComputeMemory		( Size numNodes, Size numAttributes )
{
	Size descriptorSize = sizeof( BlockDescriptor );
	Size indirectTableSize = m_numInitIndirectPtrs * sizeof( vr::OctreeFarPointer );
	Size attributesSize = numAttributes * sizeof( vr::VoxelAttributes );
	Size nodesSize = numNodes * sizeof( vr::OctreeNode );

	return descriptorSize + indirectTableSize + attributesSize + nodesSize;
}



// ================================ //
//
Size				OctreeBuilder::ComputeMaxDirectOffset()
{
	Size ones = std::numeric_limits< Size >::max();
	Size max = ones & ( ones >> 42 );

	return max;
}


}	// vr
