#pragma once
/**
@file AttributeHeader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/HierarchicalChunkedFormat/Attributes/AttributeTypes.h"

namespace sw
{

/**@brief */
struct AttributeHeader
{
	Size			AttribSize;		///< Size of attribute including this field.
	AttributeType	AttribType;		///< Attribute type. Low 16 bits are reserved fo built in attributes.


// ================================ //
//
	AttributeHeader( AttributeType type )
		:	AttribSize( 0 )
		,	AttribType( type )
	{}
};


}	// sw
