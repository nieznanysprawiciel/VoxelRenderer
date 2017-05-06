/**
@file ImplHCF.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"


#include "ImplHCF.h"




namespace sw
{

const Version currentVersion = "1.0.0.0";

// ================================ //
//
ImplHCF::ImplHCF()
	:	m_header( currentVersion )
	,	m_file( nullptr )
	,	m_writePtr( 0 )
	,	m_attributesWritten( false )
	,	m_fileReadOnly( false )
{}

// ================================ //
//
ImplHCF::~ImplHCF()
{
	if( m_file )
		fclose( m_file );
	m_file = nullptr;
}

// ================================ //
//
Chunk			ImplHCF::GetRootChunk()
{
	if( !m_rootChunk )
	{
		if( m_header.RootChunkOffset != 0 )
		{
			auto newChunk = ChunkRepr::CreateFromFile( this, nullptr, m_header.RootChunkOffset );

			if( newChunk->CheckValidity() )
				m_rootChunk = newChunk;
		}
	}

	return Chunk( m_rootChunk );
}

// ================================ //
//
bool			ImplHCF::OpenFile	( const filesystem::Path& filePath, bool writeDirect )
{
	m_file = fopen( filePath.String().c_str(), "wb" );
	
	if( m_file != nullptr )
	{
		m_directWrite = writeDirect;
		if( m_directWrite )
		{
			ReserveMemory( sizeof( FileHeader ) );
			m_header.FileSize = sizeof( FileHeader );
			m_header.RootChunkOffset = 0;

			// Write header to file. Header must be modified in future.
			fwrite( (void*)&m_header, sizeof( FileHeader ), 1, m_file );	// Should check result for safety.
		}

		return true;
	}
	
	return false;
}

// ================================ //
//
bool			ImplHCF::WriteFile	( const filesystem::Path& filePath )
{
	if( m_directWrite )
	{
		m_header.FileSize = m_writePtr;

		fseek( m_file, (long)0, SEEK_SET );
		fwrite( (void*)&m_header, sizeof( FileHeader ), 1, m_file );

		fclose( m_file );

		return true;
	}
	else
	{

	}

	return false;
}

// ================================ //
//
bool			ImplHCF::LoadFile		( const filesystem::Path& filePath, bool readOnly )
{
	m_fileReadOnly = readOnly;
	m_directWrite = false;

	const char* openMode = nullptr;
	if( m_fileReadOnly )
		openMode = "rb";
	else
		openMode = "wb";


	m_file = fopen( filePath.String().c_str(), openMode );
	if( m_file != nullptr )
	{
		fread( (void*)&m_header, sizeof( FileHeader ), 1, m_file );
		m_rootChunk = nullptr;

		return true;
	}

	return false;
}

// ================================ //
//
Chunk			ImplHCF::CreateRootChunk()
{
	if( !m_rootChunk )
	{
		if( m_directWrite )
		{
			m_header.RootChunkOffset = ReserveMemory( 0 );
			m_attributesWritten = true;
		}

		m_rootChunk = ChunkRepr::Create( this, nullptr );
		return Chunk( m_rootChunk );
	}
	else
		return Chunk( m_rootChunk );
}

// ================================ //
//
Attribute		ImplHCF::AddGlobalAttribute	( AttributeType type, const DataPtr data, Size dataSize )
{
	if( CanAddGlobalAttrib() )
	{
		Attribute newAttribute = AddAttribute( m_fileAttributes, type, data, dataSize );

		// Move offset in file header.
		Size size = ComputeWholeSize( newAttribute );
		m_header.FileSize += size;

		return newAttribute;
	}
	return Attribute();
}

// ================================ //
//
Attribute		ImplHCF::AddAttribute	( AttributeReprPtr& list, AttributeType type, const DataPtr data, Size dataSize )
{
	AttributeReprPtr attribRepr = MakePtr< AttributeRepr >( this, type );

	// Add attribute to list
	if( list )
		list->AddNextAttribute( attribRepr );
	else
		list = attribRepr;

	attribRepr->FillAttribute( data, dataSize );

	return Attribute( attribRepr );
}

// ================================ //
//
Size			ImplHCF::ComputeWholeSize	( Attribute attrib )
{
	return attrib.GetSize();
}


// ================================ //
//
Size			ImplHCF::ReserveMemory		( Size dataSize )
{
	Size curPtr = m_writePtr;
	m_writePtr += dataSize;
	
	return curPtr;
}

// ================================ //
//
bool			ImplHCF::CanAddGlobalAttrib	() const
{
	if( !m_rootChunk &&
		!m_attributesWritten )
		return true;
	return false;
}

}	// sw
