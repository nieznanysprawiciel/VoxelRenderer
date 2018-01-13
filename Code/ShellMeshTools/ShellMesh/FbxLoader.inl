#pragma once
/**
@file FbxLoader.inl
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "FBXLoader.h"
#include <string.h>		// memcpy


namespace vr
{



// ================================ //
//
template< typename VertexType >
inline void		FBXLoader::CopyVertexBuffer	( const std::vector< VertexType >& verticies, MemoryChunk& destination )
{
	auto dest = destination.GetMemory< VertexType >();
	memcpy( dest, verticies.data(), verticies.size() * sizeof( VertexType ) );
}

// ================================ //
//
template< typename IndexType >
inline void		FBXLoader::CopyIndexBuffer		( const std::vector< std::vector< Index32 > >& indicies, MemoryChunk& destination )
{
	auto dest = destination.GetMemory< IndexType >();

	for( int i = 0; i < indicies.size(); ++i )
	{
		for( int indexIdx = 0; indexIdx < indicies[ i ].size(); indexIdx++ )
		{
			*dest = (IndexType)indicies[ i ][ indexIdx ];
			dest++;
		}
	}
}


}	// sw

