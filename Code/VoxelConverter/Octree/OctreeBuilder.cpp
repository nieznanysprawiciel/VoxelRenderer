#include "OctreeBuilder.h"

#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"

#include "VoxelRenderer/SVO/OctreeNode.h"
#include "VoxelRenderer/SVO/VoxelAttributes.h"


namespace vr
{

// ================================ //
//
OctreeBuilder::OctreeBuilder()
	:	m_numInitIndirectPtrs( 512 )
	,	m_curNodesOffset( 0 )
	,	m_curIndirectOffset( 0 )
	,	m_attributesOffset( 0 )
{}

// ================================ //
//
OctreePtr			OctreeBuilder::BuildOctree		( OctreeFile& srcOctree )
{
	m_numNodes = srcOctree.node_count;
	m_numAttribs = srcOctree.data_count;

	m_octree = std::unique_ptr< ooc::Octree[] >( new ooc::Octree[ srcOctree.node_count ] );
	m_attributes = std::unique_ptr< ooc::Payload[] >( new ooc::Payload[ srcOctree.data_count ] );

	srcOctree.readNode( (byte*)m_octree.get() );
	srcOctree.readData( (byte*)m_attributes.get() );

	BuildEmptyStructure();
	BuildAttributesSegment();


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
	BlockDescriptor& desc = reinterpret_cast< BlockDescriptor& >( *m_data.data() );
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
		vr::VoxelAttributes& attribute = reinterpret_cast< vr::VoxelAttributes& >( *m_data.data() );

		attribute.Normal = payload.Normal;
		attribute.Color = DirectX::PackedVector::XMCOLOR( payload.Color.x, payload.Color.y, payload.Color.z, 1.0f );
	}
}

// ================================ //
//
void				OctreeBuilder::BuildNodeHierarchy()
{

}

// ================================ //
//
Size				OctreeBuilder::ComputeMemory	( Size numNodes, Size numAttributes )
{
	Size descriptorSize = sizeof( BlockDescriptor );
	Size indirectTableSize = m_numInitIndirectPtrs * sizeof( vr::OctreeFarPointer );
	Size attributesSize = numAttributes * sizeof( vr::VoxelAttributes );
	Size nodesSize = numNodes * sizeof( vr::OctreeNode );

	return descriptorSize + indirectTableSize + attributesSize + nodesSize;
}


}	// vr
