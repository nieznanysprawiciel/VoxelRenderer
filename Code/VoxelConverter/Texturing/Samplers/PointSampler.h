#pragma once


#include "glm/glm.hpp"

#include "VoxelConverter/Texturing/TextureAccessor.h"


/**@brief Samples one texel nearest to UV coordinate.*/
class PointSampler
{
private:
protected:
public:
	explicit		PointSampler		() = default;
	~PointSampler	() = default;


	static inline glm::vec4		Sample		( const TextureAccessor& tex, glm::vec2 uv );
};

//====================================================================================//
//			Implementation
//====================================================================================//

// ================================ //
//
inline glm::vec4		PointSampler::Sample	( const TextureAccessor& tex, glm::vec2 uv )
{
	auto texelSpace = tex.TexelSpacePosition( uv );
	texelSpace = glm::round( texelSpace );
	return tex.SampleTexel( glm::ivec2( texelSpace ) );
}
