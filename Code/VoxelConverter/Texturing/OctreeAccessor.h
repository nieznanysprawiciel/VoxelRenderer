#pragma once


#include "VoxelRenderer/SVO/Octree.h"




/**@brief */
class OctreeAccessor
{
private:

	vr::OctreePtr		m_octree;

	uint32				m_attributesOffset;


protected:
public:

	explicit		OctreeAccessor		( vr::OctreePtr octree );
					~OctreeAccessor	() = default;


	Size					GetNumberAttributes		() const;
	vr::VoxelAttributes&	GetAttributes			( Size idx );

};

