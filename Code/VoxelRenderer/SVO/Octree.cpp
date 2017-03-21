#include "Octree.h"

#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"
#include "VoxelRenderer/SVO/VoxtreeHeader.h"


namespace vr
{


// ================================ //
//
Octree::Octree()
{}

// ================================ //
//
Octree::Octree		( std::vector< OctreeNode >&& data, Size gridSize, Size firstFreeIndirectOffset )
	:	m_gridSize( gridSize )
	,	m_nodes( std::move( data ) )
	,	m_indirectPtrs( firstFreeIndirectOffset )
{
	m_sizeTotal = m_nodes.size();
}

// ================================ //
//
bool		Octree::WriteToFile		( const filesystem::Path& outputFilePath )
{
	sw::HCF hcf;
	if( hcf.OpenFile( outputFilePath, sw::HCF::WriteMode::DirectToFile ) )
	{
		vr::VoxtreeHeader header;
		header.VoxtreeVersion.Major = 1;
		header.VoxtreeVersion.Minor = 0;
		header.GridSize = m_gridSize;
		header.FreeIndirectOffset = m_indirectPtrs;

		hcf.AddAttribute( header );

		sw::Chunk root = hcf.CreateRootChunk();
		root.Fill( (sw::DataPtr)m_nodes.data(), m_nodes.size() * sizeof( vr::OctreeNode ) );

		return hcf.WriteFile( outputFilePath );
	}

	return false;
}

// ================================ //
//
bool		Octree::LoadFromFile	( const filesystem::Path& outputFilePath )
{
	return false;
}


}	// svo
