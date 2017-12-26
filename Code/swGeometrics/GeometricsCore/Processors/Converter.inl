#pragma once
/**
@file Converter.inl
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


#include "Converter.h"

#include <algorithm>
#include <numeric>

#include <assert.h>



namespace sw {
namespace geometrics
{

/**@brief Functor for comparing indicies which reference verticies in specifued vector.

This functor is used by std::sort function.*/
template< typename VertexType, typename IndexType >
class VertexComparator
{
private:
	const std::vector< VertexType >&	m_verticies;
public:
	VertexComparator ( const std::vector< VertexType >& verts ) : m_verticies( verts ) {}


	bool operator() ( int i, int j )
	{
		if( m_verticies[ i ] < m_verticies[ j ] )
			return true;
		return false;
	}
};


// ================================ //
//
template< typename VertexType, typename IndexType >
inline typename IndexedGeometry< VertexType, IndexType >		Converter::MakeIndexed( const std::vector< VertexType >& srcVerticies )
{
	IndexedGeometry< VertexType, IndexType > result;
	result.Indicies = std::move( MakeIndexed< VertexType, IndexType >( srcVerticies, result.Verticies ) );

	return result;
}

// ================================ //
//
template< typename VertexType, typename IndexType >
inline std::vector< IndexType >								Converter::MakeIndexed( const std::vector< VertexType >& srcVerticies, std::vector< VertexType >& dstVerticies )
{
	std::vector< IndexType > indicies;
	indicies.resize( srcVerticies.size() );	// Sincve we have triangle list topology, new index buffer will have the same size as srcVerticies.

	assert( dstVerticies.size() < std::numeric_limits< IndexType >::max() );
	IndexType vertexOffset = static_cast<IndexType>( dstVerticies.size() );

	std::vector< IndexType > sortedIndicies;
	sortedIndicies.resize( srcVerticies.size() );

	// Fill temporary index vector with increasing numbers. Note that this vector is index buffer for our source array.
	std::iota( sortedIndicies.begin(), sortedIndicies.end(), 0 );

	VertexComparator< VertexType, IndexType > comparator( srcVerticies );
	std::sort( sortedIndicies.begin(), sortedIndicies.end(), comparator );


	// Iterate through verticies. Equal verticies are placed near each other.
	// Merge these verticies and compute new indiecies.
	IndexType tmpIdx = 0;
	IndexType uniqueVertIdx = 0;
	while( tmpIdx < sortedIndicies.size() )
	{
		// Rewrite srcVerticies( tmpIdx ) to new buffer.
		dstVerticies.push_back( srcVerticies[ sortedIndicies[ tmpIdx ] ] );
		indicies[ sortedIndicies[ tmpIdx ] ] = uniqueVertIdx + vertexOffset;


		// Verticies are in increasing order, so this comparision checks if we are still in group of equal verticies.
		while( tmpIdx + 1 < sortedIndicies.size() && !( comparator( sortedIndicies[ tmpIdx ], sortedIndicies[ tmpIdx + 1 ] ) ) )
		{
			indicies[ sortedIndicies[ tmpIdx + 1 ] ] = uniqueVertIdx + vertexOffset;
			tmpIdx++;
		}

		uniqueVertIdx++;
		tmpIdx++;
	}

	return indicies;
}

}	// geometrics
}	// sw

