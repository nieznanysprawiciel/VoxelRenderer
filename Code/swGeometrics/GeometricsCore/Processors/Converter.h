#pragma once
/**
@file Converter.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include <vector>

#include "swGeometrics/GeometricsCore/Types/IndexedGeometry.h"



namespace sw {
namespace geometrics
{





/**@brief Converts different types of geoemtry.*/
class Converter
{
private:
protected:
public:
	explicit		Converter() = default;
	~Converter() = default;

/**@brief Function computes indexed vertex and index buffer. Each vertex is unique in created array.

Operator < for VertexType must be defined.

@param[in] srcVerticies Vector of triangles.
@return Indexed geometry.*/
	template< typename VertexType, typename IndexType >
	static IndexedGeometry< VertexType, IndexType >		MakeIndexed		( const std::vector< VertexType >& srcVerticies );

	/**@copydoc Converter::MakeIndexed

	Fills dstVerticies with unique verticies.*/
	template< typename VertexType, typename IndexType >
	static std::vector< IndexType >						MakeIndexed		( const std::vector< VertexType >& srcVerticies, std::vector< VertexType >& dstVerticies );
};


}	// geometrics
}	// sw

#include "Converter.inl"
