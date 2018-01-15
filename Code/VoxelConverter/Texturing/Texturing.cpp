#include "Texturing.h"

#include "TextureAccessor.h"
#include "OctreeAccessor.h"

#include "Samplers/Samplers.h"



// ================================ //
//
template< typename SamplerObject >
void			TextureOctreeWithSampler		( const TextureAccessor& tex, OctreeAccessor& octree, Size texIdx )
{
	/// @todo In future we should check bounds of startRange and endRange here, but current implementation iterates
	/// all attributes, so there's nothing to worry yet.

	for( Size attribIdx = 0; attribIdx < octree.GetNumberAttributes(); attribIdx++ )
	{
		auto& attributes = octree.GetAttributes( attribIdx );

		// Texture index is encoded in z coordinate of color.
		if( floor( attributes.Color.z ) == texIdx )
		{
			// We interpret colors as UVs coords. External tool should prepare SVO containing UVs.
			glm::vec2 uv( attributes.Color.x, attributes.Color.y );
			glm::vec4 color = SamplerObject::Sample( tex, uv );

			attributes.Color = DirectX::XMFLOAT3( color.x, color.y, color.z );
		}
	}
}



// ================================ //
//
Texturing::Texturing	( OctreeAccessor octree )
	:	m_octree( octree )
	,	m_wrappingModeX( WrappingMode::Mirror )
	,	m_wrappingModeY( WrappingMode::Mirror )
{}

// ================================ //
//
Texturing::Texturing	( OctreeAccessor octree, bool flipU, bool flipV )
	:	m_octree( octree )
	,	m_wrappingModeX( WrappingMode::Mirror )
	,	m_wrappingModeY( WrappingMode::Mirror )
	,	m_flipU( flipU )
	,	m_flipV( flipV )
{}

// ================================ //
//
bool		Texturing::TextureOctree		( const filesystem::Path& filePath, SamplerType samplingType, Size texIdx )
{
	if( !m_octree.IsValid() )
		return false;

	TextureAccessor textureAcc( m_wrappingModeX, m_wrappingModeY );

	if( !textureAcc.LoadImage( filePath ) )
		return false;

	switch( samplingType )
	{
		case SamplerType::Point:
			TextureOctreeWithSampler< PointSampler >( textureAcc, m_octree, texIdx );
		case SamplerType::Bilinear:
			TextureOctreeWithSampler< BilinearSampler >( textureAcc, m_octree, texIdx );
		default:
			return false;
	}

	return true;
}
