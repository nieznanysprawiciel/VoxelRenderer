/**
@file ChunkRepr.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"

#include "swCommonLib/HierarchicalChunkedFormat/Internal/ImplHCF.h"
#include "ChunkRepr.h"

#include <assert.h>


namespace sw
{

// ================================ //
//
ChunkRepr::ChunkRepr( ImplHCF* hcf, ChunkReprPtr& parent )
	:	m_hcf( hcf )
	,	m_parent( parent )
	,	m_absolutOffset( 0 )
{
	if( m_hcf->m_directWrite )
		m_absolutOffset = m_hcf->ReserveMemory( sizeof( ChunkHeader ) );
	
	// Reserve space for header and move write pointer.
	UpdateHeader();
}

// ================================ //
//
ChunkRepr::ChunkRepr( ImplHCF* hcf, ChunkReprPtr& parent, Size fileOffset )
	:	m_hcf( hcf )
	,	m_parent( parent )
	,	m_absolutOffset( fileOffset )
{
	LoadHeader( m_absolutOffset );
}

// ================================ //
//
ChunkReprPtr	ChunkRepr::CreateFromFile	( ImplHCF* hcf, ChunkReprPtr parent, Size fileOffset )
{
	return std::shared_ptr< ChunkRepr >( new ChunkRepr( hcf, parent, fileOffset ) );
}

// ================================ //
//
ChunkReprPtr	ChunkRepr::Create			( ImplHCF* hcf, ChunkReprPtr parent )
{
	return std::shared_ptr< ChunkRepr >( new ChunkRepr( hcf, parent ) );
}


// ================================ //
//
Chunk			ChunkRepr::CreateChunk		()
{
	if( CanCreateChunk() )
	{
		ChunkReprPtr newChunk = ChunkRepr::Create( m_hcf, shared_from_this() );

		if( m_childChunk )
			m_childChunk->AddNextChunk( newChunk );
		else
		{
			m_header.HasChildren = true;
			m_header.DataOffset = static_cast< uint32 >( newChunk->m_absolutOffset - m_absolutOffset );

			m_childChunk = newChunk;
			UpdateHeader();
		}

		return Chunk( newChunk );
	}
	return Chunk();
}

// ================================ //
//
Chunk			ChunkRepr::NextChunk()
{
	if( m_nextChunk )
		return Chunk( m_nextChunk );

	// Need to load chunk from file.
	if( m_header.NextChunk >= sizeof( ChunkHeader ) )
	{
		ChunkReprPtr newChunk = ChunkRepr::CreateFromFile( m_hcf, shared_from_this(), m_absolutOffset + m_header.NextChunk );
		if( newChunk->CheckValidity() )
			m_nextChunk = newChunk;
		
		return Chunk( m_nextChunk );
	}
	return Chunk();
}

// ================================ //
//
Chunk			ChunkRepr::FirstChild()
{
	if( m_childChunk )
		return Chunk( m_childChunk );

	// Need to load chunk from file.
	if( m_header.HasChildren )
	{
		if( m_header.DataOffset >= sizeof( ChunkHeader ) )
		{
			ChunkReprPtr newChunk = ChunkRepr::CreateFromFile( m_hcf, shared_from_this(), m_absolutOffset + m_header.DataOffset );
			if( newChunk->CheckValidity() )
				m_nextChunk = newChunk;

			return Chunk( m_nextChunk );
		}
	}
	return Chunk();
}

// ================================ //
//
bool			ChunkRepr::HasChildren() const
{
	return m_header.HasChildren;
}

// ================================ //
//
Chunk			ChunkRepr::ParentChunk()
{
	return Chunk( m_parent.lock() );
}

// ================================ //
//
Attribute		ChunkRepr::AddAttribute		( AttributeType type, const DataPtr data, Size dataSize )
{
	if( CanAddAttribute() )
	{
		Attribute newAttribute = m_hcf->AddAttribute( m_firstAttrib, type, data, dataSize );

		//m_header.DataOffset += (uint32)m_hcf->ComputeWholeSize( newAttribute );
		//m_header.NextChunk += m_hcf->ComputeWholeSize( newAttribute );

		return newAttribute;
	}

	// Return invalid attribute. Maybe exception ???
	return Attribute();
}

// ================================ //
//
bool			ChunkRepr::Fill				( const DataPtr data, Size dataSize )
{
	if( CanFillData() )
	{
		if( m_hcf->m_directWrite )
		{
			m_header.DataSize = dataSize;
			m_header.HasChildren = false;
			m_header.DataOffset = static_cast< uint32 >( m_hcf->ReserveMemory( dataSize ) - m_absolutOffset );

			auto file = m_hcf->GetFile();
			if( !file )
				throw std::runtime_error( "No file opened for writing." );

			//// Validate position in stream.
			//Size streamPos = ftell( file );
			//assert( m_header.DataOffset + m_absolutOffset == streamPos );

			Size written = fwrite( (void*)data, 1, dataSize, file );
			
			// @todo What to do in this case. Should we revert to previous state? Do some error handling.
			assert( written == dataSize );

			UpdateHeader();

			return true;
		}
		else
		{

		}
	}

	return false;
}

// ================================ //
//
DataUPack		ChunkRepr::StealData()
{
	if( !m_data.Data )
	{
		m_data = LoadData();
	}

	return std::move( m_data );
}

// ================================ //
//
DataPack		ChunkRepr::AccessData()
{
	if( !m_data.Data )
	{
		m_data = LoadData();
	}
	
	DataPack data;
	data.Data = m_data.Data.get();
	data.DataSize = m_data.DataSize;

	return data;
}

// ================================ //
//
void			ChunkRepr::AddNextChunk		( ChunkReprPtr& newChunk )
{
	if( m_nextChunk )
		m_nextChunk->AddNextChunk( m_nextChunk );
	else
	{
		m_nextChunk = newChunk;
		m_header.NextChunk = newChunk->m_absolutOffset - m_absolutOffset;
		UpdateHeader();
	}
}

// ================================ //
//
bool			ChunkRepr::CheckValidity	() const
{
	bool valid = true;

	if( !( m_header.ChunkHead[ 0 ] == 'C' &&
		m_header.ChunkHead[ 1 ] == 'H' && 
		m_header.ChunkHead[ 2 ] == 'N' ) )
	valid = false;

	return valid;
}


//====================================================================================//
//			Internal Helpers	
//====================================================================================//

// ================================ //
//
bool			ChunkRepr::CanAddAttribute	() const
{
	// You can always add attribute if we aren't in direct write mode.
	if( !m_hcf->m_directWrite )
		return true;

// In direct write mode:
	// Can't add attribute when child chunk or next chunk is already created.
	if( m_childChunk || m_nextChunk )
		return false;

	return true;
}

// ================================ //
//
bool			ChunkRepr::CanCreateChunk	() const
{
	// You can always create chunk if we aren't in direct write mode.
	if( !m_hcf->m_directWrite )
		return true;

	// Can't create child chunk if next chunk on the same level already exists.
	if( m_nextChunk )
		return false;

	return true;
}

// ================================ //
//
bool			ChunkRepr::CanFillData		() const
{
	// You can always create chunk if we aren't in direct write mode.
	if( !m_hcf->m_directWrite )
		return true;

	// Can't create child chunk if next chunk on the same level already exists.
	if( m_childChunk || m_nextChunk )
		return false;

	return true;
}

// ================================ //
//
bool			ChunkRepr::CanLoadData		() const
{
	// If chunk has children, can't have data.
	if( HasChildren() )
		return false;

	// We can't load data if we're in write only mode.
	if( m_hcf->m_directWrite )
		return false;

	// If DataOffset must point to free memory space. If it's smaller than ChunkHeader,
	// probably there's no data in this chunk.
	if( m_header.DataOffset < sizeof( ChunkHeader ) )
		return false;

	if( m_header.DataSize == 0 )
		return false;

	return true;
}


// ================================ //
//
void			ChunkRepr::WriteHeader		( Size revertOffset )
{
	auto file = m_hcf->GetFile();
	if( !file )
		throw std::runtime_error( "No file opened for writing." );

	fseek( file, (long)m_absolutOffset, SEEK_SET );
	fwrite( (void*)&m_header, sizeof( ChunkHeader ), 1, file );
	fseek( file, (long)revertOffset, SEEK_SET );
}

// ================================ //
//
void			ChunkRepr::LoadHeader		( Size revertOffset )
{
	auto file = m_hcf->GetFile();
	if( !file )
		throw std::runtime_error( "No file opened for reading." );

	fseek( file, (long)m_absolutOffset, SEEK_SET );
	fread( (void*)&m_header, sizeof( ChunkHeader ), 1, file );
	fseek( file, (long)revertOffset, SEEK_SET );
}

// ================================ //
//
void			ChunkRepr::UpdateHeader		()
{
	Size curPtr = m_hcf->ReserveMemory( 0 );
	WriteHeader( curPtr );
}

// ================================ //
//
DataUPack		ChunkRepr::LoadData()
{
	if( CanLoadData() )
	{
		DataUPack dataPack;
		dataPack.Data = DataUPtr( new DataUnderlayingType[ m_header.DataSize ] );
		dataPack.DataSize = m_header.DataSize;

		auto file = m_hcf->GetFile();
		if( !file )
			throw std::runtime_error( "No file opened for reading." );

		DataPtr dstPtr = dataPack.Data.get();
		//Size revertOffset = m_hcf->ReserveMemory( 0 );	// Remember current read pointer

		fseek( file, (long)m_absolutOffset + m_header.DataOffset, SEEK_SET );
		Size readBytes = fread( dstPtr, 1, dataPack.DataSize, file );

		// @todo What to do in this case. Should we revert to previous state? Do some error handling.
		if( readBytes != dataPack.DataSize )
			return DataUPack();
		
		//fseek( file, (long)revertOffset, SEEK_SET );		// Set previous file position.

		return std::move( dataPack );
	}

	return DataUPack();
}

}	// sw
