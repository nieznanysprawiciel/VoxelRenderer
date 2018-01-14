#include "TextureAccessor.h"


#define STB_IMAGE_IMPLEMENTATION
#include "Soil2/src/SOIL2/stb_image.h"

#include <math.h>


// ================================ //
//
TextureAccessor::TextureAccessor( WrappingMode modeX, WrappingMode modeY )
	:	m_textureData( nullptr )
	,	m_wrappingModeX( modeX )
	,	m_wrappingModeY( modeY )
	,	m_height( 0 )
	,	m_width( 0 )
	,	m_channels( 0 )
	,	m_flipU( false )
	,	m_flipV( true )
{}

// ================================ //
//
TextureAccessor::~TextureAccessor()
{
	if( m_textureData )
		stbi_image_free( m_textureData );
}

// ================================ //
//
bool			TextureAccessor::LoadImage		( const filesystem::Path& filePath )
{
	if( !filePath.Exists() )
		return false;

	m_textureData = stbi_load( filePath.String().c_str(), &m_width, &m_height, &m_channels, 4 );
	m_channels = 4;

	if( !m_textureData )
		return false;

	return true;
}

// ================================ //
//
glm::vec2		TextureAccessor::TexelSpacePosition		( glm::vec2 uv ) const
{
	uv.x = ApplyWrapping( m_wrappingModeX, uv.x );
	uv.y = ApplyWrapping( m_wrappingModeY, uv.y );

	if( m_flipU ) uv.x = 1.0f - uv.x;
	if( m_flipV ) uv.y = 1.0f - uv.y;

	return uv * glm::vec2( m_width - 1, m_height - 1 );
}

// ================================ //
//
glm::vec4		TextureAccessor::SampleTexel			( glm::ivec2 texelPos ) const
{
	texelPos = glm::clamp( texelPos, glm::ivec2( 0, 0 ), glm::ivec2( m_width - 1, m_height - 1 ) );

	PtrOffset offset = m_width * texelPos.y * m_channels + texelPos.x * m_channels;
	uint8* texelPtr = m_textureData + offset;

	return DispatchLittleEndian( texelPtr );
}


// ================================ //
//
float			TextureAccessor::ApplyWrapping		( WrappingMode mode, float coord ) const
{
	switch( mode )
	{
		case WrappingMode::Clamp:
			return glm::clamp( coord, 0.0f, 1.0f );
		case WrappingMode::Mirror:
		{
			float clamped = fmod( coord, 2.0f );
			if( clamped > 1.0f )
				return 2.0f - clamped;
			return clamped;
		}
		case WrappingMode::Repeat:
			return fmod( coord, 1.0f );
		default:
			break;
	}

	return 0.0f;
}

// ================================ //
//
glm::vec4		TextureAccessor::DispatchLittleEndian	( uint8* texelPtr ) const
{
	// Zero color vector.
	glm::vec4 color( 0.0f, 0.0f, 0.0f, 1.0f );

	// This dispatches texel written in little endian.
	int channelOffset = 0;
	for( int channel = m_channels - 1; channel >= 0; --channel, channelOffset++ )
	{
		auto texelChannelPtr = texelPtr + channelOffset;
		uint8 sample = *texelChannelPtr;
		color[ channel ] = (float)sample / 255.0f;
	}

	return color;
}

// ================================ //
//
glm::vec4		TextureAccessor::DispatchBigEndian		( uint8* texelPtr ) const
{
	// Zero color vector.
	glm::vec4 color( 0.0f, 0.0f, 0.0f, 1.0f );

	// This dispatches texel written in little endian.
	int channelOffset = 0;
	for( int channel = 0; channel < m_channels; ++channel, channelOffset++ )
	{
		auto texelChannelPtr = texelPtr + channelOffset;
		uint8 sample = *texelChannelPtr;
		color[ channel ] = (float)sample / 255.0f;
	}

	return color;
}


