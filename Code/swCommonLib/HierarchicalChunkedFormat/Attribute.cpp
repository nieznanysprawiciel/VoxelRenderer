/**
@file Attribute.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"

#include "swCommonLib/HierarchicalChunkedFormat/Internal/AttributeRepr.h"
#include "Attribute.h"


namespace sw
{

// ================================ //
//
Attribute::Attribute	( AttributeReprPtr attribPtr )
	: m_attribPtr( attribPtr )
{}


// ================================ //
//
bool				Attribute::IsValid() const
{
	if( m_attribPtr )
		return true;
	return false;
}

// ================================ //
//
AttributeType		Attribute::GetType() const
{
	if( m_attribPtr )
		return m_attribPtr->AccessHeader().AttribType;
	return AttributeTypeBuiltIn::InvalidAttribute;
}

// ================================ //
//
Size				Attribute::GetSize() const
{
	return m_attribPtr->AccessHeader().AttribSize;
}


}	// sw
