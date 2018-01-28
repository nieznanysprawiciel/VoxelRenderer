#include "VoxelConverter.h"


#include "ooc_svo_builder/src/svo_builder/octree_io.h"

#include "Octree/OctreeBuilder.h"
#include "VoxelRenderer/SVO/VoxtreeHeader.h"
#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"
#include "swCommonLib/System/Dir.h"

#include <fstream>



// ================================ //
//
VoxelConverter::VoxelConverter()
	:	m_samplerType( SamplerType::Bilinear )
	,	m_applyTextureToAll( false )
{}



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
	m_texturesPaths.push_back( texPath );
}

// ================================ //
//
void				VoxelConverter::LoadMatListFile	( const filesystem::Path& matlistPath )
{
	std::fstream file( matlistPath.String(), std::fstream::in );
	if( file.is_open() )
	{
		auto matlistDir = matlistPath.GetDirectory();

		std::string texturePath;
		while( std::getline( file, texturePath ) )
		{
			// Absolut paths are always invalid. In such a case we can try to use only
			// file name and hope the file exists.
			auto texPath = filesystem::Path( texturePath );
			if( texPath.IsAbsolut() )
				texPath = texPath.GetFileName();

			auto finalPath = matlistDir / texturePath;

			m_texturesPaths.push_back( finalPath );
			texturePath.clear();
		}

		file.close();
	}
}

// ================================ //
//
bool				VoxelConverter::SetSampler	( const std::string& samplerName )
{
	if( samplerName == "Bilinear" )
		SetSampler( SamplerType::Bilinear );
	else if( samplerName == "Point" )
		SetSampler( SamplerType::Point );
	else
		return false;

	return true;
}

// ================================ //
//
void				VoxelConverter::ApplyTextures	( vr::OctreeBuilder& builder )
{
	Size texIdx = 0;
	for( auto& texture : m_texturesPaths )
	{
		if( !texture.String().empty() )
			builder.TextureOctree( texture, m_samplerType, texIdx, m_flipU, m_flipV );

		texIdx++;
	}
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
		ApplyTextures( builder );
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





