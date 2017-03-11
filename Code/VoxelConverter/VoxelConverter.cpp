#include "VoxelConverter.h"


#include "liboctree/include/octree_io.h"

#include "Octree/OctreeBuilder.h"



// ================================ //
//
bool			VoxelConverter::Convert		( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath )
{
	vr::OctreePtr octree = Load( inputFilePath );
	return Write( outputFilePath, octree );
}


// ================================ //
//
vr::OctreePtr		VoxelConverter::Load		( const filesystem::Path& inputFilePath )
{
	auto srcOctree = readOctreeFile( inputFilePath.String() );

	vr::OctreeBuilder builder;
	return builder.BuildOctree( srcOctree );
}

// ================================ //
//
bool			VoxelConverter::Write		( const filesystem::Path& outputFilePath, vr::OctreePtr octree )
{
	return false;
}





