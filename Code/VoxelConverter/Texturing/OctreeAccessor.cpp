#include "OctreeAccessor.h"

#include "VoxelRenderer/SVO/OctreeNode.h"


// ================================ //
//
OctreeAccessor::OctreeAccessor( ooc::OctreeNode* nodes, ooc::Payload* attribs, std::vector< bool >& texturedNodes, uint64 numNodes, uint64 numAttribs )
	:	m_nodes( nodes )
	,	m_attributes( attribs )
	,	m_texturedNodes( texturedNodes )
	,	m_numNodes( numNodes )
	,	m_numAttributes( numAttribs )
{}

// ================================ //
//
Size							OctreeAccessor::GetNumberAttributes	() const
{
	return m_numAttributes;
}

// ================================ //
//
ooc::Payload&					OctreeAccessor::GetAttributes		( Size idx )
{
	assert( idx < m_numAttributes );

	return m_attributes[ idx ];
}

// ================================ //
//
bool							OctreeAccessor::IsValid				() const
{
	if( m_attributes && m_nodes )
		return true;

	return false;
}
