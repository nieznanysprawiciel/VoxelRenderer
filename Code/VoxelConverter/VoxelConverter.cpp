#include "VoxelConverter.h"


#include "ooc_svo_builder/src/svo_builder/octree_io.h"

#include "Octree/OctreeBuilder.h"
#include "VoxelRenderer/SVO/VoxtreeHeader.h"
#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"
#include "swCommonLib/System/Dir.h"



// ================================ //
//
bool				VoxelConverter::Convert		( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath )
{
	vr::OctreePtr octree = Load( inputFilePath );
	return Write( outputFilePath, octree );
}

// ================================ //
//
void				VoxelConverter::AddTexture	( const filesystem::Path& texPath )
{
	m_texturePath = texPath;
}


// ================================ //
//
vr::OctreePtr		VoxelConverter::Load		( const filesystem::Path& inputFilePath )
{
	OctreeInfo srcOctree;
	parseOctreeHeader( inputFilePath.String(), srcOctree );

	vr::OctreeBuilder builder;

	if( builder.ReadOctree( srcOctree ) )
	{
		if( !m_texturePath.String().empty() )
			builder.TextureOctree( m_texturePath, SamplerType::Point );

		return builder.BuildOctree();
	}

	return nullptr;
}

// ================================ //
//
bool			VoxelConverter::Write		( const filesystem::Path& outputFilePath, vr::OctreePtr octree )
{
	filesystem::Dir::CreateDirectory( outputFilePath );
	return octree->WriteToFile( outputFilePath );
}





