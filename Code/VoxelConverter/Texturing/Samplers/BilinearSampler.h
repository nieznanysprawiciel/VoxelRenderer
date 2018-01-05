#pragma once


#include "glm/glm.hpp"

#include "VoxelConverter/Texturing/TextureAccessor.h"


/**@brief Samples one texel nearest to UV coordinate.*/
class BilinearSampler
{
private:
protected:
public:
	explicit		BilinearSampler		() = default;
	~BilinearSampler	() = default;


	static inline glm::vec4		Sample		( const TextureAccessor& tex, glm::vec2 uv );
};

//====================================================================================//
//			Implementation
//====================================================================================//

// ================================ //
//
inline glm::vec4		BilinearSampler::Sample	( const TextureAccessor& tex, glm::vec2 uv )
{
	auto texelSpace = tex.TexelSpacePosition( uv );

	float xFactor = fmod( texelSpace.x, 1.0f );
	float yFactor = fmod( texelSpace.y, 1.0f );

	int minCoordX = (int)floor( texelSpace.x );
	int minCoordY = (int)floor( texelSpace.y );

	glm::vec4 samples[ 4 ];

	samples[ 0 ] = tex.SampleTexel( glm::ivec2( minCoordX, minCoordY ) );
	samples[ 1 ] = tex.SampleTexel( glm::ivec2( minCoordX + 1, minCoordY ) );
	samples[ 2 ] = tex.SampleTexel( glm::ivec2( minCoordX, minCoordY + 1 ) );
	samples[ 3 ] = tex.SampleTexel( glm::ivec2( minCoordX + 1, minCoordY + 1 ) );

	return samples[ 0 ] * ( 1.0f - xFactor ) * ( 1.0f - yFactor )
		+ samples[ 1 ] * xFactor * ( 1.0f - yFactor )
		+ samples[ 2 ] * ( 1.0f - xFactor ) * yFactor
		+ samples[ 3 ] * xFactor * yFactor;
}


