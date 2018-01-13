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


// ================================ //
//
template< typename VertexType >
inline
void			FBXLoader::TransformVerticies	( std::vector< VertexType >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix )
{
	XMMATRIX transform = XMLoadFloat4x4( &matrix );

	for( uint32 i = offset; i < verticies.size(); ++i )
	{
		auto& vertex = verticies[ i ];

		XMVECTOR position = XMLoadFloat3( &vertex.Position );
		position = XMVector3Transform( position, transform );
		XMStoreFloat3( &vertex.Position, position );

		XMVECTOR normal = XMLoadFloat3( &vertex.Normal );
		normal = XMVector3TransformNormal( normal, transform );
		XMStoreFloat3( &vertex.Normal, normal );
	}
}

}	// sw

