#pragma once

#include "VoxelRenderer/SVO/Octree.h"

#include "swCommonLib/System/Path.h"


/**@brief */
class VoxelConverter
{
private:
protected:
public:
	explicit		VoxelConverter		() = default;
					~VoxelConverter		() = default;

	vr::OctreePtr	Load				( const filesystem::Path& inputFilePath );
	bool			Write				( const filesystem::Path& outputFilePath, vr::OctreePtr octree );

	bool			Convert				( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath );

};




