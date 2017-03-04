/**
@file ChunkNameAttrib.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"
#include "ChunkNameAttrib.h"

#include <assert.h>


namespace sw
{

// ================================ //
//
ChunkNameAttrib::ChunkNameAttrib		( const std::string& name )
	:	ChunkName( name )
{}

// ================================ //
//
ChunkNameAttrib::ChunkNameAttrib		( std::string&& name )
	:	ChunkName( std::move( name ) )
{}

// ================================ //
//
bool		ChunkNameAttrib::ParseMemory	( DataPtr data, Size size )
{
	ChunkName = std::string( (const char*)data, size );
	return true;
}

// ================================ //
//
bool		ChunkNameAttrib::Save			( DataPtr data, Size size )
{
	assert( size == ChunkName.length() + 1 );

	memcpy( data, ChunkName.c_str(), size );
	return true;
}

// ================================ //
//
Size		ChunkNameAttrib::ComputeSize	()
{
	// We save string with null terminator.
	return ChunkName.length() + 1;
}

// ================================ //
//
AttributeType	ChunkNameAttrib::GetType()
{
	return AttributeTypeBuiltIn::ChunkName;
}

}	// sw
