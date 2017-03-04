/**
@file AttributeRepr.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/
#include "swCommonLib/HierarchicalChunkedFormat/stdafx.h"

#include "swCommonLib/HierarchicalChunkedFormat/Internal/ImplHCF.h"
#include "AttributeRepr.h"



namespace sw
{

// ================================ //
//
AttributeRepr::AttributeRepr( ImplHCF* hcf, AttributeType type )
	:	m_header( AttributeHeader( type ) )
	,	m_hcf( hcf )
{}

// ================================ //
//
void		AttributeRepr::AddNextAttribute		( AttributeReprPtr& attribPtr )
{
	if( m_nextAttrib )
		m_nextAttrib->AddNextAttribute( attribPtr );
	else
		m_nextAttrib = attribPtr;
}

// ================================ //
//
void		AttributeRepr::FillAttribute		( const DataPtr data, Size dataSize )
{
	if( m_hcf->m_directWrite )
	{
		auto file = m_hcf->GetFile();
		if( !file )
			throw std::runtime_error( "No file opened for writing." );

		m_absolutOffset = m_hcf->ReserveMemory( sizeof( AttributeHeader ) + dataSize );

		fwrite( (void*)&AccessHeader(), sizeof( AttributeHeader ), 1, file );
		fwrite( data, dataSize, 1, file );
	}
	else
	{

	}
}

}	// sw

