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
const OctreeNode&		Octree::GetNode			( uint32 absolutOffset )
{
	return m_nodes[ absolutOffset ];
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
	sw::HCF hcf;
	if( hcf.LoadFile( outputFilePath, true ) )
	{
		// @todo: Check VoxtreeHeader attribute. In this moment it's imposible, because HCF has no API for this.

		sw::Chunk root = hcf.GetRootChunk();
		if( root.IsValid() )
		{
			// @todo HCF should support loading data to memory address given by external code.
			// We must have two memory buffers at the same time. This can be too much in case of voxels.
			auto data = root.AccessData();
			m_nodes.resize( data.DataSize / sizeof( vr::OctreeNode ) );

			memcpy( m_nodes.data(), data.Data, data.DataSize );

			return true;
		}
	}

	return false;
}


}	// svo
