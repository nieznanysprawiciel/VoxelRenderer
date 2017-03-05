#include "VoxelConverter.h"


#include "liboctree/include/octree_io.h"





// ================================ //
//
vr::OctreePtr		VoxelConverter::Load		( const filesystem::Path& inputFilePath )
{
	auto srcOctree = readOctreeFile( inputFilePath.String() );


	return vr::OctreePtr();
}

// ================================ //
//
bool			VoxelConverter::Write		( const filesystem::Path& outputFilePath, vr::OctreePtr octree )
{
	return false;
}

// ================================ //
//
bool			VoxelConverter::Convert		( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath )
{
	vr::OctreePtr octree = Load( inputFilePath );
	return Write( outputFilePath, octree );
}



