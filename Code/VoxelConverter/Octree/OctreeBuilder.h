#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include "ooc_svo_builder/src/svo_builder/octree_io.h"

#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"

#include "VoxelRenderer/SVO/Octree.h"


namespace vr
{

/**@brief */
class OctreeBuilder
{
private:

	Size			m_numInitIndirectPtrs;
	Size			m_maxDirectOffset;

	Size			m_numNodes;
	Size			m_numAttribs;

	std::unique_ptr< ooc::OctreeNode[] >	m_octree;
	std::unique_ptr< ooc::Payload[] >		m_attributes;

	std::vector< OctreeNode >				m_data;

	Size			m_curNodesOffset;
	Size			m_curIndirectOffset;
	Size			m_attributesOffset;

protected:
public:
	explicit		OctreeBuilder		();
					~OctreeBuilder		() = default;

	OctreePtr		BuildOctree			( OctreeInfo& srcOctree );

private:

	Size				ComputeMemory			( Size numNodes, Size numAttributes );
	void				BuildEmptyStructure		();
	void				BuildAttributesSegment	();
	void				BuildNodeHierarchy		();
	void				BuildNodeHierarchy		( ooc::OctreeNode& srcNode, Size srcOffset, vr::OctreeNode& dstNode );

	bool				IsLeafNode				( ooc::OctreeNode& srcNode );
	int8				CountChildren			( ooc::OctreeNode& srcNode );
	uint8				InverseCoords			( uint8 childShift );
	void				SetChildMask			( ooc::OctreeNode& srcNode, vr::OctreeNode& dstNode );

	VoxelAttributes&	AccessAttributes		( uint64 dataOffset );
	vr::OctreeNode&		AccessNode				( uint32 absolutOffset );
	ooc::OctreeNode&	AccessNode				( ooc::OctreeNode& parent, uint8 index );
	OctreeFarPointer&	AccessFarPtr			( uint32 absolutOffset );
	Size				AllocateNodes			( uint8 numNodes );
	uint32				AllocateIndirect		();
	uint32				ComputeAttribOffset		( uint64 dataOffset );

	ooc::OctreeNode&	AccessNext				( ooc::OctreeNode& parent, int8& startIdx );

	Size				ComputeMaxDirectOffset	();
};


}

