#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include "liboctree/include/octree_io.h"

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

	Size			m_numNodes;
	Size			m_numAttribs;

	std::unique_ptr< ooc::Octree[] >		m_octree;
	std::unique_ptr< ooc::Payload[] >		m_attributes;

	std::vector< OctreeNode >				m_data;

	Size			m_curNodesOffset;
	Size			m_curIndirectOffset;
	Size			m_attributesOffset;

protected:
public:
	explicit		OctreeBuilder		();
					~OctreeBuilder		() = default;

	OctreePtr		BuildOctree			( OctreeFile& srcOctree );

private:

	Size			ComputeMemory			( Size numNodes, Size numAttributes );
	void			BuildEmptyStructure		();
	void			BuildAttributesSegment	();
	void			BuildNodeHierarchy		();
};


}

