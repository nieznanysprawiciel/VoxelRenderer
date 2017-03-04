#include "VoxelConverter.h"


#include "liboctree/include/octree_io.h"





// ================================ //
//
svo::OctreePtr		VoxelConverter::Load		( const filesystem::Path& inputFilePath )
{
	auto srcOctree = readOctreeFile( inputFilePath.String() );


	return svo::OctreePtr();
}

// ================================ //
//
bool			VoxelConverter::Write		( const filesystem::Path& outputFilePath, svo::OctreePtr octree )
{
	return false;
}

// ================================ //
//
bool			VoxelConverter::Convert		( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath )
{
	svo::OctreePtr octree = Load( inputFilePath );
	return Write( outputFilePath, octree );
}



