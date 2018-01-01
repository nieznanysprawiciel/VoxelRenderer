#include "Texturing.h"

#include "TextureAccessor.h"
#include "OctreeAccessor.h"

#include "Samplers/Samplers.h"



// ================================ //
//
template< typename SamplerObject >
void			TextureOctreeWithSampler		( const TextureAccessor& tex, OctreeAccessor& octree, Size startRange, Size endRange )
{


	for( Size attrib = startRange; attrib < endRange; attrib++ )
	{


	}
}



// ================================ //
//
Texturing::Texturing	( vr::OctreePtr octree )
	:	m_octree( octree )
	,	m_wrappingModeX( WrappingMode::Mirror )
	,	m_wrappingModeY( WrappingMode::Mirror )
{}

// ================================ //
//
bool		Texturing::TextureOctree		( const filesystem::Path& filePath, SamplerType samplingType )
{
	if( !m_octree )
		return false;

	TextureAccessor textureAcc( m_wrappingModeX, m_wrappingModeY );

	if( !textureAcc.LoadImage( filePath ) )
		return false;

	OctreeAccessor octreeAcc( m_octree );

	switch( samplingType )
	{
		case SamplerType::Point:
			TextureOctreeWithSampler< PointSampler >( textureAcc, octreeAcc, 0, octreeAcc.GetNumberAttributes() );
		case SamplerType::Box:
			return false;
		default:
			return false;
	}

	return true;
}
