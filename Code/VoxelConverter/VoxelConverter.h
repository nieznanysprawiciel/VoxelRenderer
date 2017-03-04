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

	svo::OctreePtr	Load				( const filesystem::Path& inputFilePath );
	bool			Write				( const filesystem::Path& outputFilePath, svo::OctreePtr octree );

	bool			Convert				( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath );
};




