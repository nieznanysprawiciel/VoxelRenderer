#include "OctreeAccessor.h"




// ================================ //
//
OctreeAccessor::OctreeAccessor( vr::OctreePtr octree )
	:	m_octree( octree )
{
	m_attributesOffset = octree->GetBlockDescriptor().AttributesOffset;
}

// ================================ //
//
Size							OctreeAccessor::GetNumberAttributes	() const
{
	return Size();
}

// ================================ //
//
vr::VoxelAttributes&			OctreeAccessor::GetAttributes		( Size idx )
{
	// TODO: insert return statement here
}
