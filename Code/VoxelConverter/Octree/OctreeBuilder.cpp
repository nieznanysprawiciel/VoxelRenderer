#include "OctreeBuilder.h"

#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"

#include "VoxelRenderer/SVO/OctreeNode.h"
#include "VoxelRenderer/SVO/VoxelAttributes.h"

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
	,	m_maxDirectOffset( ComputeMaxDirectOffset() )
{}

// ================================ //
//
OctreePtr			OctreeBuilder::BuildOctree		( OctreeInfo& srcOctree )
{
	if( srcOctree.filesExist() )
	{
		m_numNodes = srcOctree.n_nodes;
		m_numAttribs = srcOctree.n_data;

		m_octree = std::unique_ptr< ooc::OctreeNode[] >( new ooc::OctreeNode[ m_numNodes ] );
		m_attributes = std::unique_ptr< ooc::Payload[] >( new ooc::Payload[ m_numAttribs ] );

		ReadNodes( srcOctree, (uint8*)m_octree.get() );
		ReadData( srcOctree, (uint8*)m_attributes.get() );

		BuildEmptyStructure();
		BuildAttributesSegment();
		BuildNodeHierarchy();

		OctreePtr octree = std::make_shared< Octree >( std::move( m_data ), srcOctree.gridlength, m_curIndirectOffset );
		return octree;
	}

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
		leaf.IsLeafNode = true;
		leaf.AttributesOffset = ComputeAttribOffset( srcNode.DataAddress );
	}
	else
	{
		dstNode.IsLeafNode = false;
		SetChildMask( srcNode, dstNode );
		
		Size absolutOffset = AllocateNodes( numChildren );
		Size nodesOffset = absolutOffset - srcOffset;

		if( nodesOffset > m_maxDirectOffset )
		{
			// Make indirect pointer.
			dstNode.IndirectPtr = true;
			assert( !"Implement me" );
			//absolutOffset = 
		}
		else
		{
			dstNode.IndirectPtr = false;
			dstNode.ChildPackPtr = nodesOffset;
		}

		uint8 srcChildIdx = 0;
		for( int i = 0; i < numChildren; ++i )
		{
			vr::OctreeNode& child = AccessNode( (uint32)absolutOffset );
			ooc::OctreeNode& srcChild = AccessNext( srcNode, srcChildIdx );

			BuildNodeHierarchy( srcChild, absolutOffset, child );

			absolutOffset++;
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

	return !hasChild;
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
void				OctreeBuilder::SetChildMask			( ooc::OctreeNode& srcNode, vr::OctreeNode& dstNode )
{
	uint8 childMask = 1;
	for( auto child : srcNode.ChildOffset )
	{
		if( child >= 0 )
			dstNode.ChildMask = dstNode.ChildMask | childMask;

		childMask = childMask << 1;
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
Size				OctreeBuilder::AllocateNodes		( uint8 numNodes )
{
	Size firstNodeOffset = m_curNodesOffset;
	m_curNodesOffset += numNodes * sizeof( vr::OctreeNode );
	return firstNodeOffset;
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
ooc::OctreeNode&	OctreeBuilder::AccessNext			( ooc::OctreeNode& parent, uint8& startIdx )
{
	for( ; startIdx < 8; ++startIdx )
	{
		if( parent.ChildOffset[ startIdx ] >= 0 )
		{
			Size offset = parent.ChildrenBaseOffset + parent.ChildOffset[ startIdx ];
			return m_octree[ offset ];
		}
	}

	assert( false );
	//throw std::runtime_error( "Trying to access node after end of children array." );
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
