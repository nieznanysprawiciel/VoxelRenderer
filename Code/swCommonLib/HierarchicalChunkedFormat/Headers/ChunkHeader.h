#pragma once
/**
@file ChunkHeader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"


namespace sw
{


/**@brief */
struct ChunkHeader
{
	char			ChunkHead[ 3 ];	///< Letters CHN.
	bool			HasChildren;	///< Indicated wheter DataOffset points to nested chunk or data.
	uint32			DataOffset;		///< Data or nested chunks.
	Size			NextChunk;		///< Offset to Next chunk on the same level.
	Size			DataSize;		///< Size of data. If value is equal to 0

// ================================ //
//
	ChunkHeader()
		:	DataOffset( 0 )
		,	NextChunk( 0 )
		,	DataSize( 0 )
		,	HasChildren( false )
	{
		ChunkHead[ 0 ] = 'C';
		ChunkHead[ 1 ] = 'H';
		ChunkHead[ 2 ] = 'N';
	}

};

}	// sw
