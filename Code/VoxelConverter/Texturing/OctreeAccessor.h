#pragma once


#include "VoxelConverter/Octree/oocOctree.h"
#include "VoxelConverter/Octree/oocPayload.h"




/**@brief */
class OctreeAccessor
{
private:

	ooc::OctreeNode*	m_nodes;
	ooc::Payload*		m_attributes;

	uint64				m_numNodes;
	uint64				m_numAttributes;

protected:
public:

	explicit		OctreeAccessor		( ooc::OctreeNode* nodes, ooc::Payload* attribs, uint64 numNodes, uint64 numAttribs );
					~OctreeAccessor	() = default;


	Size					GetNumberAttributes		() const;
	ooc::Payload&			GetAttributes			( Size idx );

	bool					IsValid					() const;

};

