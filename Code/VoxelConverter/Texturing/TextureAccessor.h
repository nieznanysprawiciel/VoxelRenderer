#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/System/Path.h"

#include "TexturingEnums.h"

#include "glm/glm.hpp"


/**@brief Wrapps texture and makes it easier to sample.*/
class TextureAccessor
{
private:

	uint8*			m_textureData;

	WrappingMode	m_wrappingModeX;
	WrappingMode	m_wrappingModeY;

	int				m_height;
	int				m_width;
	int				m_channels;

protected:
public:
	explicit		TextureAccessor		( WrappingMode modeX, WrappingMode modeY );
					~TextureAccessor	();

	bool			LoadImage			( const filesystem::Path& filePath );

	glm::vec2		TexelSpacePosition	( glm::vec2 uv ) const;
	glm::vec4		SampleTexel			( glm::ivec2 texelPos ) const;

private:

	float			ApplyWrapping		( WrappingMode mode, float coord ) const;
};



