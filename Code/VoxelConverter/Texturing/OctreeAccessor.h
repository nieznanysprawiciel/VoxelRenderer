#pragma once


#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"


#include <vector>



/**@brief */
class OctreeAccessor
{
private:

	ooc::OctreeNode*	m_nodes;
	ooc::Payload*		m_attributes;

	std::vector< bool >&	m_texturedNodes;		///< Because we make multiple passes through octree and we modify it, we must flag processed entries.


	uint64				m_numNodes;
	uint64				m_numAttributes;

protected:
public:

	explicit		OctreeAccessor		( ooc::OctreeNode* nodes, ooc::Payload* attribs, std::vector< bool >& texturedNodes, uint64 numNodes, uint64 numAttribs );
					~OctreeAccessor	() = default;


	Size					GetNumberAttributes		() const;
	ooc::Payload&			GetAttributes			( Size idx );

	std::vector< bool >&	GetTexturedNodesFlags	() const { return m_texturedNodes; }

	bool					IsValid					() const;

};

