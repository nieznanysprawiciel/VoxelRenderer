#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"

#include "OctreeNode.h"
#include "VoxelAttributes.h"

#include <vector>

class Octree;
DEFINE_PTR_TYPE( Octree );
DEFINE_WPTR_TYPE( Octree );


/**@brief Octree structure.

Octrees can be nested. Order of octrees in m_subtrees array matters. Links between octrees are built by OctreeFarPointer references.
This reference is offset to m_nodes array in different tree. When OctreeFarPointer is larger then size of this octree, we have to
search list of subtrees. Use GetSubtreeTotalSize to determine size of subtree (with all it's children).

Whole subtree can be threatet as flat list of nodes.

OctreeFarPointer should reference only direct children.
*/
class Octree
{
private:

	OctreeWPtr			m_parent;			///< Parent of this octree. Can be nullptr.
	OctreeFarPointer*	m_parentLink;		///< Node which points to this octree in parent structure.

	Size				m_sizeTotal;		///< Includes size of nested subtrees to faster traversal.
	Size				m_gridSize;			///< Power of 2.

	std::vector< OctreeNode >		m_nodes;		///< This can be OctreeNode, OctreeFarPointer, or VoxelAttributes
	std::vector< OctreePtr >		m_subtrees;

protected:
public:
	explicit		Octree();
	~Octree() = default;


	Size			GetSubtreeTotalSize		() { return m_sizeTotal; }
	Size			GetGridSize				() { return m_gridSize; }
};


