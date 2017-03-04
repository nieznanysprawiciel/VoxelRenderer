#pragma once

#include "HCF.h"


namespace sw
{

// ================================ //
//
template< typename AttributeStruct >
inline Attribute			HCF::AddAttribute	( AttributeType type, const AttributeStruct& content )
{
	return m_impl.AddGlobalAttribute( type, (const DataPtr)&content, sizeof( AttributeStruct ) );
}

// ================================ //
//
template< typename AttributeStruct >
inline Attribute			HCF::AddAttribute	( const AttributeStruct& content )
{
	return AddAttribute( GetAttributeTypeID< AttributeStruct >(), content );
}



}	// sw

